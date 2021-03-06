
/*
 * 
 * (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
 *
 */


/**
 * @file rw_fsm.h
 * @author Vinod Kamalaraj
 * @date 03/11/2014
 * @brief RIFT FSM library 
 * @details A FSM library that provides framework for timers and statistics.
 */

#ifndef __RW_FSM_H
#define __RW_FSM_H

#include <ctype.h>
#include <stdint.h>

__BEGIN_DECLS


  
#include "rwlib.h"
// #include "rwlog.h"
#define RW_FSM_MAX_TRACE_ELTS 10
/**
 * Handle for a FSM
 */

typedef void * rw_fsm_handle_t;    
    

/**
 * Type of a function that is called on entering or exiting  state in the FSM
 */
typedef void (*rw_state_entry_exit_function_t) (rw_fsm_handle_t handle, /*< Handle on which the FSM is working on */
                                                uint8_t state,          /*!< state that is entered or exited */
                                                uint8_t is_entry);      /*!< true if the state is being entered */

/**
 * The profile for a state in an FSM
 */

typedef struct _rw_fsm_state_profile {
  const char *name;                       /*< Name of this state */
  uint16_t statistics;                    /*< Statistics associated with this state */
  uint8_t timer;                          /*< Enumeration of timer that needs to run on this state */
  rw_state_entry_exit_function_t entry;   /*< Function to be executed on entering this state */
  rw_state_entry_exit_function_t exit;    /*< Function to be executed on exiting this state */
} rw_fsm_state_profile_t;

/**
 * The profile for an event in an FSM
 */

typedef struct _rw_fsm_event_profile {
  const char *name;                       /*< Name of this event */
  uint16_t statistics;                    /*< Statistics associated with this event */
} rw_fsm_event_profile_t;


/**
 * Sources for events in rw fsm world
 */

typedef enum _rw_event_protocol {
  RW_EVENT_PROTOCOL_INVALID,              /*< Invalid event - prevent unfilled events from being processed */
  RW_EVENT_PROTOCOL_NETCONF,              /*< Events from the NETCONF protocol */
  RW_EVENT_PROTOCOL_INTERNAL_APP,         /*< Event generated by an RW APP */
  RW_EVENT_PROTOCOL_INTERNAL_TEST,        /*< Events generated for unit tests */
} rw_event_protocol_t;

/**
 * The disposition of an event - what happened to an event in a handler function
 */
typedef enum _rw_event_disposition {
  RW_EVENT_DISPOSITION_INVALID,         /*< The disposition hasnt set by the event handler */
  RW_EVENT_DISPOSITION_PROCESSED,       /*< The event was processed by the function */
  RW_EVENT_DISPOSITION_IGNORED,         /*< The event was ignored by the FSM */
} rw_event_disposition_t;

/**
 * The disposition of a handle - has anything happened to the handle in a handler function?
 */

typedef enum _rw_handle_disposition {
  RW_HANDLE_DISPOSITION_NONE,         /*< No change has happened to the handle */
  RW_HANDLE_DISPOSITION_DESTROYED,    /*< The handle has been destroyed by the event handler function */
} rw_handle_disposition_t;

/**
 * An event in the FSM framework
 */ 

typedef struct _rw_fsm_event {
  uint8_t                     rfe_type;              /*< The event typed mapped for this FSM */
  rw_event_protocol_t         rfe_protocol;          /*< Layer from where this event originated */
  void                        *rfe_event;             /*< The event as originated outside of this FSM */
} rw_fsm_event_t;

/**
 * Returned data from a event handle
 */

typedef struct _rw_fsm_eh_return {
  rw_event_disposition_t      rer_ev_disposition;      /*< Event disposition from the handler */
  rw_handle_disposition_t     rer_handle_disposition;  /*< Disposition of the handler */
} rw_fsm_eh_return_t;

/**
 * Global data needed within the FSM - Logging context, timer control block etc
 * Maybe these should be configurable in the FSM context later?
 */

typedef struct _rw_fsm_global_data {
  //  rwlog_ctx_t                 rgd_log_ctxt;           /*< Logging context for the context*/
} rw_fsm_global_data_t;


/**
 * Definition of a event handler function
 */

typedef rw_status_t (*rw_fsm_eh_t) (rw_fsm_handle_t        handle,   /*< Handle on which the FSM operates */
                                    rw_fsm_event_t         *event,    /*< Incoming Event */
                                    rw_fsm_global_data_t   *glob,     /*< required global data for handler function */
                                    rw_fsm_eh_return_t     *ret);     /*< returned values from the handler */

/**
 * FSM trace element
 */

typedef struct _rw_fsm_trace_element {
  uint8_t             rft_state;                      /*< State in the trace */
  uint8_t             rft_mapped_event;               /*< Event as seen by the FSM */
  rw_event_protocol_t rft_protocol;                   /*< Protocol on which the event was received */
  uint16_t            rft_protocol_event;             /*< event type within the protocol */
  uint32_t            rft_time_stamp;                 /*< Time stamp of event handling */
} rw_fsm_trace_element_t;

/**
 * FSM trace 
 */

typedef struct _rw_fsm_trace {
  uint8_t                 rft_curr_idx;                    /*< Current index in the trace structure */
  rw_fsm_trace_element_t  rft_elts[RW_FSM_MAX_TRACE_ELTS]; /*< The trace elements  */
} rw_fsm_trace_t;
  
/**
 * FSM data within a handle or object
 */

typedef struct _rw_fsm_state_data {
  uint8_t          rsd_state;                   /*< The current state of the FSM */
  uint8_t          rsd_in_eh;                   /*< Is this FSM in a event handler? */
  rw_fsm_trace_t   rsd_trace;                   /*< FSM trace */  
} rw_fsm_state_data_t;

/**
 * Type for function that maps a timer enum to a value that can be used in the timer function
 */

typedef rw_status_t (*rw_fsm_timer_map_fn_t) (uint8_t fsm_enum, uint32_t *timer_val);

/**
 * Type for function that maps a protocol event to a FSM event 
 */

typedef rw_status_t (*rw_fsm_event_map_fn_t) (rw_fsm_event_t *event);

/**
 * FSM object
 */

typedef struct _rw_fsm {
  uint8_t                  rfs_state_count;       /*< Number of states in the FSM */
  uint8_t                  rfs_event_count;       /*< Number of events in the FSM */
  size_t                   rfs_offset;            /*< Offset within handle of state data */
  rw_fsm_state_profile_t   *rfs_state_prfl;        /*< Profiles of the states */
  rw_fsm_event_profile_t   *rfs_event_prfl;        /*< Profiles of the events */
  rw_fsm_timer_map_fn_t    rfs_timer_map;         /*< Map timer enums to values */
  rw_fsm_event_map_fn_t    rfs_event_map;         /*< Get FSM enums for current event */
  rw_fsm_eh_t              *rfs_eh;               /*< Event handlers for expected events */
  rw_fsm_eh_t              rfs_unexp_eh;         /*< All unexpected events */
} rw_fsm_t;

/**
 * Sending an event to a FSM 
 */

rw_status_t rw_fsm_handle_event (rw_fsm_t              *fsm,      /*< FSM for which the event is destined to*/
				 rw_fsm_handle_t       hndl,      /*< Handle on which to run the FSM on */
				 rw_event_protocol_t   protocol,  /*< Protocol from which the event was received */
				 rw_fsm_global_data_t  *glob,     
				 void                  *event,   /*< Event within that protocol */
				 rw_fsm_eh_return_t    *ret);


__END_DECLS    

#endif /* __RW_FSM_H */

