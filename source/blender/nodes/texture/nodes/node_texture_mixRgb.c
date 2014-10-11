/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Robin Allen
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/nodes/texture/nodes/node_texture_mixRgb.c
 *  \ingroup texnodes
 */


#include "node_texture_util.h"
#include "NOD_texture.h"

/* **************** MIX RGB ******************** */
static bNodeSocketTemplate inputs[] = {
	{ SOCK_FLOAT, 1, N_("Factor"), 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_NONE },
	{ SOCK_RGBA,  1, N_("Color1"), 0.5f, 0.5f, 0.5f, 1.0f },
	{ SOCK_RGBA, 1, N_("Color2"), 0.5f, 0.5f, 0.5f, 1.0f },
	{ -1, 0, "" }
};
static bNodeSocketTemplate outputs[] = {
	{ SOCK_RGBA, 0, N_("Color") },
	{ -1, 0, "" }
};

static void exec(void *UNUSED(data),
                 int UNUSED(thread),
                 bNode *node,
                 bNodeExecData *UNUSED(execdata),
                 bNodeStack **in,
                 bNodeStack **out)
{
	float fac  = tex_input_value(in[0]);
	float col1[4], col2[4];

	tex_input_rgba(col1, in[1]);
	tex_input_rgba(col2, in[2]);

	/* use alpha */
	if (node->custom2 & 1)
		fac *= col2[3];

	CLAMP(fac, 0.0f, 1.0f);

	copy_v4_v4(out[0]->vec, col1);
	ramp_blend(node->custom1, out[0]->vec, fac, col2);
}

void register_node_type_tex_mix_rgb(void)
{
	static bNodeType ntype;

	tex_node_type_base(&ntype, TEX_NODE_MIX_RGB, "Mix", NODE_CLASS_OP_COLOR, 0);
	node_type_socket_templates(&ntype, inputs, outputs);
	node_type_label(&ntype, node_blend_label);
	node_type_exec(&ntype, NULL, NULL, exec);

	nodeRegisterType(&ntype);
}
