import collections
import socket
import subprocess
import time

import pytest

import gi
gi.require_version('RwMcYang', '1.0')
gi.require_version('RwNsrYang', '1.0')
from gi.repository import (
        NsdYang,
        RwConmanYang,
        RwMcYang,
        RwNsrYang,
        VlrYang,
        RwVlrYang,
        RwVnfdYang,
        RwVnfrYang,
        VnfrYang
        )
import rift.auto.session
import rift.mano.examples.ping_pong_nsd as ping_pong


@pytest.fixture(scope='module')
def proxy(request, launchpad_session):
    return launchpad_session.proxy


def yield_vnfd_vnfr_pairs(proxy, nsr=None):
    """
    Yields tuples of vnfd & vnfr entries.

    Args:
        proxy (callable): Launchpad proxy
        nsr (optional): If specified, only the vnfr & vnfd records of the NSR
                are returned

    Yields:
        Tuple: VNFD and its corresponding VNFR entry
    """
    def get_vnfd(vnfd_id):
        xpath = "/vnfd-catalog/vnfd[id='{}']".format(vnfd_id)
        return proxy(RwVnfdYang).get(xpath)

    vnfr = "/vnfr-catalog/vnfr"
    vnfrs = proxy(RwVnfrYang).get(vnfr, list_obj=True)
    for vnfr in vnfrs.vnfr:

        if nsr:
            const_vnfr_ids = [const_vnfr.vnfr_id for const_vnfr in nsr.constituent_vnfr_ref]
            if vnfr.id not in const_vnfr_ids:
                continue

        vnfd = get_vnfd(vnfr.vnfd_ref)
        yield vnfd, vnfr


def yield_nsd_nsr_pairs(proxy):
    """Yields tuples of NSD & NSR pairs

    Args:
        proxy (callable): Launchpad proxy

    Yields:
        Tuple: NSD and its corresponding NSR record
    """
    nsr = "/ns-instance-opdata/nsr"
    nsrs = proxy(RwNsrYang).get(nsr, list_obj=True)
    for nsr in nsrs.nsr:
        nsd_path = "/ns-instance-config/nsr[id='{}']".format(
                nsr.ns_instance_config_ref)
        nsd = proxy(RwNsrYang).get_config(nsd_path)

        yield nsd, nsr


def assert_records(proxy):
    """Verifies if the NSR & VNFR records are created
    """
    ns_tuple = list(yield_nsd_nsr_pairs(proxy))
    assert len(ns_tuple) == 1

    vnf_tuple = list(yield_vnfd_vnfr_pairs(proxy))
    assert len(vnf_tuple) == 2


@pytest.mark.depends('nsr')
@pytest.mark.incremental
class TestRecords(object):
    def is_valid_ip(self, address):
        """Verifies if it is a valid IP and if its accessible

        Args:
            address (str): IP address

        Returns:
            boolean
        """
        try:
            socket.inet_aton(address)
        except socket.error:
            return False
        else:
            return True

    def test_records_present(self, proxy):
        assert_records(proxy)

    def test_vdu_record_params(self, proxy):
        """
        Asserts:
        1. If a valid floating IP has been assigned to the VM
        3. Check if the VM flavor has been copied over the VDUR
        """
        for vnfd, vnfr in yield_vnfd_vnfr_pairs(proxy):
            assert vnfd.mgmt_interface.port == vnfr.mgmt_interface.port

            for vdud, vdur in zip(vnfd.vdu, vnfr.vdur):
                assert vdud.vm_flavor == vdur.vm_flavor
                assert self.is_valid_ip(vdur.management_ip) is True
                assert vdud.external_interface[0].vnfd_connection_point_ref == \
                    vdur.external_interface[0].vnfd_connection_point_ref

    def test_external_vl(self, proxy):
        """
        Asserts:
        1. Valid IP for external connection point
        2. A valid external network fabric
        3. Connection point names are copied over
        """
        for vnfd, vnfr in yield_vnfd_vnfr_pairs(proxy):
            cp_des, cp_rec = vnfd.connection_point, vnfr.connection_point
            assert cp_des[0].name == cp_rec[0].name
            assert self.is_valid_ip(cp_rec[0].ip_address) is True

            xpath = "/vlr-catalog/vlr[id='{}']/network-id".format(cp_rec[0].vlr_ref)
            network_id = proxy(VlrYang).get(xpath)
            assert len(network_id) > 0

    def test_monitoring_params(self, proxy):
        """
        Asserts:
        1. The value counter ticks?
        2. If the meta fields are copied over
        """
        def mon_param_record(vnfr_id, mon_param_id):
             return '/vnfr-catalog/vnfr[id="{}"]/monitoring-param[id="{}"]'.format(
                    vnfr_id, mon_param_id)

        for vnfd, vnfr in yield_vnfd_vnfr_pairs(proxy):
            for mon_des in (vnfd.monitoring_param):
                mon_rec = mon_param_record(vnfr.id, mon_des.id)
                mon_rec = proxy(VnfrYang).get(mon_rec)

                # Meta data check
                fields = mon_des.as_dict().keys()
                for field in fields:
                    assert getattr(mon_des, field) == getattr(mon_rec, field)
                # Tick check
                #assert mon_rec.value_integer > 0

    def test_nsr_record(self, proxy):
        """
        Currently we only test for the components of NSR tests. Ignoring the
        operational-events records

        Asserts:
        1. The constituent components.
        2. Admin status of the corresponding NSD record.
        """
        for nsd, nsr in yield_nsd_nsr_pairs(proxy):
            # 1 n/w and 2 connection points
            assert len(nsr.vlr) == 1
            assert len(nsr.vlr[0].vnfr_connection_point_ref) == 2

            assert len(nsr.constituent_vnfr_ref) == 2
            assert nsd.admin_status == 'ENABLED'

    def test_create_update_vnfd(self, proxy, ping_pong_records):
        """
        Verify VNFD related operations

        Asserts:
            If a VNFD record is created
        """
        ping_vnfd, pong_vnfd, _ = ping_pong_records
        vnfdproxy = proxy(RwVnfdYang)

        for vnfd_record in [ping_vnfd, pong_vnfd]:
            xpath = "/vnfd-catalog/vnfd[id='{}']".format(vnfd_record.id)
            vnfdproxy.replace_config(xpath, vnfd_record.vnfd)
            vnfd = vnfdproxy.get(xpath)
            assert vnfd.id == vnfd_record.id

    def test_create_update_nsd(self, proxy, ping_pong_records):
        """
        Verify NSD related operations

        Asserts:
            If NSD record was created
        """
        _, _, ping_pong_nsd = ping_pong_records
        nsdproxy = proxy(NsdYang)

        xpath = "/nsd-catalog/nsd[id='{}']".format(ping_pong_nsd.id)
        nsdproxy.replace_config(xpath, ping_pong_nsd.descriptor)
        nsd = nsdproxy.get(xpath)
        assert nsd.id == ping_pong_nsd.id

    def test_wait_for_pingpong_configured(self, proxy):
        nsr_opdata = proxy(RwNsrYang).get('/ns-instance-opdata')
        nsrs = nsr_opdata.nsr

        assert len(nsrs) == 1
        current_nsr = nsrs[0]

        xpath = "/ns-instance-opdata/nsr[ns-instance-config-ref='{}']/config-status".format(current_nsr.ns_instance_config_ref)
        proxy(RwNsrYang).wait_for(xpath, "configured", timeout=240)

    def test_cm_nsr(self, proxy):
        """
        Asserts:
            1. The ID of the NSR in cm-state
            2. Name of the cm-nsr
            3. The vnfr component's count
            4. State of the cm-nsr
        """
        for nsd, _ in yield_nsd_nsr_pairs(proxy):
            con_nsr_xpath = "/cm-state/cm-nsr[id='{}']".format(nsd.id)
            con_data = proxy(RwConmanYang).get(con_nsr_xpath)

            assert con_data is not None, \
                    "No Config data obtained for the nsd {}: {}".format(
                    nsd.name, nsd.id)
            assert con_data.name == "ping_pong_nsd"
            assert len(con_data.cm_vnfr) == 2

            state_path = con_nsr_xpath + "/state"
            proxy(RwConmanYang).wait_for(state_path, 'ready', timeout=120)

    def test_cm_vnfr(self, proxy):
        """
        Asserts:
            1. The ID of Vnfr in cm-state
            2. Name of the vnfr
            3. State of the VNFR
            4. Checks for a reachable IP in mgmt_interface
            5. Basic checks for connection point and cfg_location.
        """
        def is_reachable(ip, timeout=10):
            rc = subprocess.call(["ping", "-c1", "-w", str(timeout), ip])
            if rc == 0:
                return True
            return False

        nsd, _ = list(yield_nsd_nsr_pairs(proxy))[0]
        con_nsr_xpath = "/cm-state/cm-nsr[id='{}']".format(nsd.id)

        for _, vnfr in yield_vnfd_vnfr_pairs(proxy):
            con_vnfr_path = con_nsr_xpath + "/cm-vnfr[id='{}']".format(vnfr.id)
            con_data = proxy(RwConmanYang).get(con_vnfr_path)

            assert con_data is not None

            state_path = con_vnfr_path + "/state"
            proxy(RwConmanYang).wait_for(state_path, 'ready', timeout=120)

            con_data = proxy(RwConmanYang).get(con_vnfr_path)
            assert is_reachable(con_data.mgmt_interface.ip_address) is True

            assert len(con_data.connection_point) == 1
            connection_point = con_data.connection_point[0]
            assert connection_point.name == vnfr.connection_point[0].name
            assert connection_point.ip_address == vnfr.connection_point[0].ip_address

            assert con_data.cfg_location is not None

@pytest.mark.depends('nsr')
@pytest.mark.incremental
class TestNfviMetrics(object):

    def test_records_present(self, proxy):
        assert_records(proxy)

    @pytest.mark.skipif(True, reason='NFVI metrics collected from NSR are deprecated, test needs to be updated to collected metrics from VNFRs')
    def test_nfvi_metrics(self, proxy):
        """
        Verify the NFVI metrics

        Asserts:
            1. Computed metrics, such as memory, cpu, storage and ports, match
               with the metrics in NSR record. The metrics are computed from the
               descriptor records.
            2. Check if the 'utilization' field has a valid value (> 0) and matches
               with the 'used' field, if available.
        """
        for nsd, nsr in yield_nsd_nsr_pairs(proxy):
            nfvi_metrics = nsr.nfvi_metrics
            computed_metrics = collections.defaultdict(int)

            # Get the constituent VNF records.
            for vnfd, vnfr in yield_vnfd_vnfr_pairs(proxy, nsr):
                vdu = vnfd.vdu[0]
                vm_spec = vdu.vm_flavor
                computed_metrics['vm'] += 1
                computed_metrics['memory'] += vm_spec.memory_mb * (10**6)
                computed_metrics['storage'] += vm_spec.storage_gb * (10**9)
                computed_metrics['vcpu'] += vm_spec.vcpu_count
                computed_metrics['external_ports'] += len(vnfd.connection_point)
                computed_metrics['internal_ports'] += len(vdu.internal_connection_point)

            assert nfvi_metrics.vm.active_vm == computed_metrics['vm']

            # Availability checks
            for metric_name in computed_metrics:
                metric_data = getattr(nfvi_metrics, metric_name)
                total_available = getattr(metric_data, 'total', None)

                if total_available is not None:
                    assert computed_metrics[metric_name] == total_available

            # Utilization checks
            for metric_name in ['memory', 'storage', 'vcpu']:
                metric_data = getattr(nfvi_metrics, metric_name)

                utilization = metric_data.utilization
                # assert utilization > 0

                # If used field is available, check if it matches with utilization!
                total = metric_data.total
                used = getattr(metric_data, 'used', None)
                if used is not None:
                    assert total > 0
                    computed_utilization = round((used/total) * 100, 2)
                    assert abs(computed_utilization - utilization) <= 0.1
