/* valaccodeelementaccess.c generated by valac, the Vala compiler
 * generated from valaccodeelementaccess.vala, do not modify */

/* valaccodememberaccess.vala
 *
 * Copyright (C) 2006  Raffaele Sandrini
 * Copyright (C) 2006-2008  Jürg Billeter
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Author:
 * 	Raffaele Sandrini <raffaele@sandrini.ch>
 *	Jürg Billeter <j@bitron.ch>
 */

#include <glib.h>
#include <glib-object.h>
#include <stdlib.h>
#include <string.h>


#define VALA_TYPE_CCODE_NODE (vala_ccode_node_get_type ())
#define VALA_CCODE_NODE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), VALA_TYPE_CCODE_NODE, ValaCCodeNode))
#define VALA_CCODE_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), VALA_TYPE_CCODE_NODE, ValaCCodeNodeClass))
#define VALA_IS_CCODE_NODE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VALA_TYPE_CCODE_NODE))
#define VALA_IS_CCODE_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), VALA_TYPE_CCODE_NODE))
#define VALA_CCODE_NODE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), VALA_TYPE_CCODE_NODE, ValaCCodeNodeClass))

typedef struct _ValaCCodeNode ValaCCodeNode;
typedef struct _ValaCCodeNodeClass ValaCCodeNodeClass;
typedef struct _ValaCCodeNodePrivate ValaCCodeNodePrivate;

#define VALA_TYPE_CCODE_WRITER (vala_ccode_writer_get_type ())
#define VALA_CCODE_WRITER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), VALA_TYPE_CCODE_WRITER, ValaCCodeWriter))
#define VALA_CCODE_WRITER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), VALA_TYPE_CCODE_WRITER, ValaCCodeWriterClass))
#define VALA_IS_CCODE_WRITER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VALA_TYPE_CCODE_WRITER))
#define VALA_IS_CCODE_WRITER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), VALA_TYPE_CCODE_WRITER))
#define VALA_CCODE_WRITER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), VALA_TYPE_CCODE_WRITER, ValaCCodeWriterClass))

typedef struct _ValaCCodeWriter ValaCCodeWriter;
typedef struct _ValaCCodeWriterClass ValaCCodeWriterClass;

#define VALA_TYPE_CCODE_EXPRESSION (vala_ccode_expression_get_type ())
#define VALA_CCODE_EXPRESSION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), VALA_TYPE_CCODE_EXPRESSION, ValaCCodeExpression))
#define VALA_CCODE_EXPRESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), VALA_TYPE_CCODE_EXPRESSION, ValaCCodeExpressionClass))
#define VALA_IS_CCODE_EXPRESSION(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VALA_TYPE_CCODE_EXPRESSION))
#define VALA_IS_CCODE_EXPRESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), VALA_TYPE_CCODE_EXPRESSION))
#define VALA_CCODE_EXPRESSION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), VALA_TYPE_CCODE_EXPRESSION, ValaCCodeExpressionClass))

typedef struct _ValaCCodeExpression ValaCCodeExpression;
typedef struct _ValaCCodeExpressionClass ValaCCodeExpressionClass;
typedef struct _ValaCCodeExpressionPrivate ValaCCodeExpressionPrivate;

#define VALA_TYPE_CCODE_ELEMENT_ACCESS (vala_ccode_element_access_get_type ())
#define VALA_CCODE_ELEMENT_ACCESS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), VALA_TYPE_CCODE_ELEMENT_ACCESS, ValaCCodeElementAccess))
#define VALA_CCODE_ELEMENT_ACCESS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), VALA_TYPE_CCODE_ELEMENT_ACCESS, ValaCCodeElementAccessClass))
#define VALA_IS_CCODE_ELEMENT_ACCESS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VALA_TYPE_CCODE_ELEMENT_ACCESS))
#define VALA_IS_CCODE_ELEMENT_ACCESS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), VALA_TYPE_CCODE_ELEMENT_ACCESS))
#define VALA_CCODE_ELEMENT_ACCESS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), VALA_TYPE_CCODE_ELEMENT_ACCESS, ValaCCodeElementAccessClass))

typedef struct _ValaCCodeElementAccess ValaCCodeElementAccess;
typedef struct _ValaCCodeElementAccessClass ValaCCodeElementAccessClass;
typedef struct _ValaCCodeElementAccessPrivate ValaCCodeElementAccessPrivate;
#define _vala_ccode_node_unref0(var) ((var == NULL) ? NULL : (var = (vala_ccode_node_unref (var), NULL)))

struct _ValaCCodeNode {
	GTypeInstance parent_instance;
	volatile int ref_count;
	ValaCCodeNodePrivate * priv;
};

struct _ValaCCodeNodeClass {
	GTypeClass parent_class;
	void (*finalize) (ValaCCodeNode *self);
	void (*write) (ValaCCodeNode* self, ValaCCodeWriter* writer);
	void (*write_declaration) (ValaCCodeNode* self, ValaCCodeWriter* writer);
	void (*write_combined) (ValaCCodeNode* self, ValaCCodeWriter* writer);
};

struct _ValaCCodeExpression {
	ValaCCodeNode parent_instance;
	ValaCCodeExpressionPrivate * priv;
};

struct _ValaCCodeExpressionClass {
	ValaCCodeNodeClass parent_class;
	void (*write_inner) (ValaCCodeExpression* self, ValaCCodeWriter* writer);
};

struct _ValaCCodeElementAccess {
	ValaCCodeExpression parent_instance;
	ValaCCodeElementAccessPrivate * priv;
};

struct _ValaCCodeElementAccessClass {
	ValaCCodeExpressionClass parent_class;
};

struct _ValaCCodeElementAccessPrivate {
	ValaCCodeExpression* _container;
	ValaCCodeExpression* _index;
};


static gpointer vala_ccode_element_access_parent_class = NULL;

gpointer vala_ccode_node_ref (gpointer instance);
void vala_ccode_node_unref (gpointer instance);
GParamSpec* vala_param_spec_ccode_node (const gchar* name, const gchar* nick, const gchar* blurb, GType object_type, GParamFlags flags);
void vala_value_set_ccode_node (GValue* value, gpointer v_object);
void vala_value_take_ccode_node (GValue* value, gpointer v_object);
gpointer vala_value_get_ccode_node (const GValue* value);
GType vala_ccode_node_get_type (void) G_GNUC_CONST;
gpointer vala_ccode_writer_ref (gpointer instance);
void vala_ccode_writer_unref (gpointer instance);
GParamSpec* vala_param_spec_ccode_writer (const gchar* name, const gchar* nick, const gchar* blurb, GType object_type, GParamFlags flags);
void vala_value_set_ccode_writer (GValue* value, gpointer v_object);
void vala_value_take_ccode_writer (GValue* value, gpointer v_object);
gpointer vala_value_get_ccode_writer (const GValue* value);
GType vala_ccode_writer_get_type (void) G_GNUC_CONST;
GType vala_ccode_expression_get_type (void) G_GNUC_CONST;
GType vala_ccode_element_access_get_type (void) G_GNUC_CONST;
#define VALA_CCODE_ELEMENT_ACCESS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), VALA_TYPE_CCODE_ELEMENT_ACCESS, ValaCCodeElementAccessPrivate))
enum  {
	VALA_CCODE_ELEMENT_ACCESS_DUMMY_PROPERTY
};
ValaCCodeElementAccess* vala_ccode_element_access_new (ValaCCodeExpression* cont, ValaCCodeExpression* i);
ValaCCodeElementAccess* vala_ccode_element_access_construct (GType object_type, ValaCCodeExpression* cont, ValaCCodeExpression* i);
ValaCCodeExpression* vala_ccode_expression_construct (GType object_type);
void vala_ccode_element_access_set_container (ValaCCodeElementAccess* self, ValaCCodeExpression* value);
void vala_ccode_element_access_set_index (ValaCCodeElementAccess* self, ValaCCodeExpression* value);
static void vala_ccode_element_access_real_write (ValaCCodeNode* base, ValaCCodeWriter* writer);
ValaCCodeExpression* vala_ccode_element_access_get_container (ValaCCodeElementAccess* self);
void vala_ccode_expression_write_inner (ValaCCodeExpression* self, ValaCCodeWriter* writer);
void vala_ccode_writer_write_string (ValaCCodeWriter* self, const gchar* s);
ValaCCodeExpression* vala_ccode_element_access_get_index (ValaCCodeElementAccess* self);
void vala_ccode_node_write (ValaCCodeNode* self, ValaCCodeWriter* writer);
static void vala_ccode_element_access_finalize (ValaCCodeNode* obj);


ValaCCodeElementAccess* vala_ccode_element_access_construct (GType object_type, ValaCCodeExpression* cont, ValaCCodeExpression* i) {
	ValaCCodeElementAccess* self = NULL;
	ValaCCodeExpression* _tmp0_ = NULL;
	ValaCCodeExpression* _tmp1_ = NULL;
	g_return_val_if_fail (cont != NULL, NULL);
	g_return_val_if_fail (i != NULL, NULL);
	self = (ValaCCodeElementAccess*) vala_ccode_expression_construct (object_type);
	_tmp0_ = cont;
	vala_ccode_element_access_set_container (self, _tmp0_);
	_tmp1_ = i;
	vala_ccode_element_access_set_index (self, _tmp1_);
	return self;
}


ValaCCodeElementAccess* vala_ccode_element_access_new (ValaCCodeExpression* cont, ValaCCodeExpression* i) {
	return vala_ccode_element_access_construct (VALA_TYPE_CCODE_ELEMENT_ACCESS, cont, i);
}


static void vala_ccode_element_access_real_write (ValaCCodeNode* base, ValaCCodeWriter* writer) {
	ValaCCodeElementAccess * self;
	ValaCCodeExpression* _tmp0_ = NULL;
	ValaCCodeWriter* _tmp1_ = NULL;
	ValaCCodeWriter* _tmp2_ = NULL;
	ValaCCodeExpression* _tmp3_ = NULL;
	ValaCCodeWriter* _tmp4_ = NULL;
	ValaCCodeWriter* _tmp5_ = NULL;
	self = (ValaCCodeElementAccess*) base;
	g_return_if_fail (writer != NULL);
	_tmp0_ = self->priv->_container;
	_tmp1_ = writer;
	vala_ccode_expression_write_inner (_tmp0_, _tmp1_);
	_tmp2_ = writer;
	vala_ccode_writer_write_string (_tmp2_, "[");
	_tmp3_ = self->priv->_index;
	_tmp4_ = writer;
	vala_ccode_node_write ((ValaCCodeNode*) _tmp3_, _tmp4_);
	_tmp5_ = writer;
	vala_ccode_writer_write_string (_tmp5_, "]");
}


ValaCCodeExpression* vala_ccode_element_access_get_container (ValaCCodeElementAccess* self) {
	ValaCCodeExpression* result;
	ValaCCodeExpression* _tmp0_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = self->priv->_container;
	result = _tmp0_;
	return result;
}


static gpointer _vala_ccode_node_ref0 (gpointer self) {
	return self ? vala_ccode_node_ref (self) : NULL;
}


void vala_ccode_element_access_set_container (ValaCCodeElementAccess* self, ValaCCodeExpression* value) {
	ValaCCodeExpression* _tmp0_ = NULL;
	ValaCCodeExpression* _tmp1_ = NULL;
	g_return_if_fail (self != NULL);
	_tmp0_ = value;
	_tmp1_ = _vala_ccode_node_ref0 (_tmp0_);
	_vala_ccode_node_unref0 (self->priv->_container);
	self->priv->_container = _tmp1_;
}


ValaCCodeExpression* vala_ccode_element_access_get_index (ValaCCodeElementAccess* self) {
	ValaCCodeExpression* result;
	ValaCCodeExpression* _tmp0_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = self->priv->_index;
	result = _tmp0_;
	return result;
}


void vala_ccode_element_access_set_index (ValaCCodeElementAccess* self, ValaCCodeExpression* value) {
	ValaCCodeExpression* _tmp0_ = NULL;
	ValaCCodeExpression* _tmp1_ = NULL;
	g_return_if_fail (self != NULL);
	_tmp0_ = value;
	_tmp1_ = _vala_ccode_node_ref0 (_tmp0_);
	_vala_ccode_node_unref0 (self->priv->_index);
	self->priv->_index = _tmp1_;
}


static void vala_ccode_element_access_class_init (ValaCCodeElementAccessClass * klass) {
	vala_ccode_element_access_parent_class = g_type_class_peek_parent (klass);
	VALA_CCODE_NODE_CLASS (klass)->finalize = vala_ccode_element_access_finalize;
	g_type_class_add_private (klass, sizeof (ValaCCodeElementAccessPrivate));
	VALA_CCODE_NODE_CLASS (klass)->write = vala_ccode_element_access_real_write;
}


static void vala_ccode_element_access_instance_init (ValaCCodeElementAccess * self) {
	self->priv = VALA_CCODE_ELEMENT_ACCESS_GET_PRIVATE (self);
}


static void vala_ccode_element_access_finalize (ValaCCodeNode* obj) {
	ValaCCodeElementAccess * self;
	self = G_TYPE_CHECK_INSTANCE_CAST (obj, VALA_TYPE_CCODE_ELEMENT_ACCESS, ValaCCodeElementAccess);
	_vala_ccode_node_unref0 (self->priv->_container);
	_vala_ccode_node_unref0 (self->priv->_index);
	VALA_CCODE_NODE_CLASS (vala_ccode_element_access_parent_class)->finalize (obj);
}


/**
 * Represents an access to an array member in the C code.
 */
GType vala_ccode_element_access_get_type (void) {
	static volatile gsize vala_ccode_element_access_type_id__volatile = 0;
	if (g_once_init_enter (&vala_ccode_element_access_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (ValaCCodeElementAccessClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) vala_ccode_element_access_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (ValaCCodeElementAccess), 0, (GInstanceInitFunc) vala_ccode_element_access_instance_init, NULL };
		GType vala_ccode_element_access_type_id;
		vala_ccode_element_access_type_id = g_type_register_static (VALA_TYPE_CCODE_EXPRESSION, "ValaCCodeElementAccess", &g_define_type_info, 0);
		g_once_init_leave (&vala_ccode_element_access_type_id__volatile, vala_ccode_element_access_type_id);
	}
	return vala_ccode_element_access_type_id__volatile;
}



