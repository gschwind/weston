/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string.h>

#include "compositor-drm.h"
#include "compositor-drm-static.h"

struct weston_drm_backend_config *
weston_drm_backend_config_create() {
	struct weston_drm_backend_config * ths =
			(__typeof__(ths))malloc(sizeof *ths);

	if(!ths)
		return 0;

	wl_list_init(&ths->outputs_list);

	ths->connector = 0;
	ths->tty = 0;
	ths->seat_id = NULL;
	ths->format = NULL;
	ths->use_pixman = false;

	return ths;

}

void
weston_drm_backend_config_clear_outputs_setup(struct weston_drm_backend_config * ths) {
	struct weston_drm_backend_output_config * pos;
	struct weston_drm_backend_output_config * tmp;
	wl_list_for_each_safe(pos, tmp, &ths->outputs_list, link) {
		wl_list_remove(&pos->link);
		free(pos->name);
		free(pos->format);
		free(pos->seat);
		free(pos->modeline);
		free(pos);
	}
}

void
weston_drm_backend_config_destroy(struct weston_drm_backend_config * ths) {
	if(!ths)
		return;

	free(ths->seat_id);
	free(ths->format);

	weston_drm_backend_config_clear_outputs_setup(ths);

	free(ths);

}

void
weston_drm_backend_config_set_connector(
		struct weston_drm_backend_config * ths,
		int connector) {
	ths->connector = connector;
}

/** The tty to be used. default: use the current tty. */
void
weston_drm_backend_config_set_tty(
		struct weston_drm_backend_config * ths,
		int tty) {
	ths->tty = tty;
}

/** If true the pixman renderer will be used instead of the OpenGL ES
 * renderer. */
void weston_drm_backend_config_set_use_pixman(
		struct weston_drm_backend_config * ths,
		bool x) {
	ths->use_pixman = x;
}

/** The seat to be used for input and output. If NULL the default "seat0"
 * will be used.
 * The backend will take ownership of the seat_id pointer and will free
 * it on backend destruction. */
void
weston_drm_backend_config_set_seat_id(
		struct weston_drm_backend_config * ths,
		char const * seat_id) {
	if(seat_id)
		ths->seat_id = strdup(seat_id);
	else
		ths->seat_id = strdup("seat0");
}

/** The pixel format of the framebuffer to be used. Valid values are:
 * - NULL - The default format ("xrgb8888") will be used;
 * - "xrgb8888";
 * - "rgb565"
 * - "xrgb2101010"
 * The backend will take ownership of the format pointer and will free
 * it on backend destruction. */
/* TODO: enums ? */
void
weston_drm_backend_config_set_format(
		struct weston_drm_backend_config * ths,
		char const * format) {
	if(ths->format)
		ths->format = strdup(format);
	else
		ths->format = strdup("xrgb8888");
}

void
weston_drm_backend_config_set_use_current_mode(
		struct weston_drm_backend_config * ths,
		bool x) {
	ths->use_current_mode = x;
}

bool
weston_drm_backend_config_get_use_current_mode(
		struct weston_drm_backend_config * ths) {
	return ths->use_current_mode;
}

void
weston_drm_backend_add_output_setup(
		struct weston_drm_backend_config * ths,
		char const * name,
		int scale,
		uint32_t transform,
		char const * format,
		char const * seat,
		enum weston_drm_backend_output_mode mode,
		char const * modeline
) {

	/* TODO: set default values of NULL parameter */

	struct weston_drm_backend_output_config * output =
			(__typeof__(output))zalloc(sizeof *output);

	wl_list_init(&output->link);

	output->name = strdup(name);
	output->base.scale = scale;
	output->base.transform = transform;

	if(format)
		output->format = strdup(format);

	if(seat)
		output->seat = strdup(seat);

	output->mode = mode;

	if(modeline)
		output->modeline = strdup(modeline);

	wl_list_insert(&ths->outputs_list, &output->link);

}

/* new load function that need weston_backend_config */
int
weston_drm_backend_load(struct weston_compositor *compositor,
		 int *argc, char **argv, struct weston_config *wc,
		 struct weston_drm_backend_config *wbc)
{
	int (*backend_init)(struct weston_compositor *c,
			    int *argc, char *argv[],
			    struct weston_config *config,
			    struct weston_backend_config *config_base);

	backend_init = weston_load_module("drm-backend.so", "backend_init");
	if (!backend_init)
		return -1;

	return backend_init(compositor, argc, argv, wc, &wbc->base);
}




