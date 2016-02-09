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

	ths->connector = 0;
	ths->tty = 0;
	ths->seat_id = "seat0";
	ths->format = 0;
	ths->configure_output = 0;
	ths->use_pixman = false;

	return ths;

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
	weston_log("XXXX\n");
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

/** Callback used to configure the outputs. This function will be called
 * by the backend when a new DRM output needs to be configured. */
void
weston_drm_backend_config_set_configure_output_func(
		struct weston_drm_backend_config * ths,
		weston_drm_backend_configure_output_func func) {
	ths->configure_output = func;
}

struct weston_backend_config *
weston_drm_backend_config_get_weston_backend_config(
		struct weston_drm_backend_config *ths) {
	return &ths->base;
}

void
weston_drm_backend_output_config_set_scale(
		struct weston_drm_backend_output_config *ths,
		uint32_t scale) {
	ths->base.scale = scale;
}

void
weston_drm_backend_output_config_set_transform(
		struct weston_drm_backend_output_config *ths,
		uint32_t transform) {
	ths->base.transform = transform;
}

void
weston_drm_backend_output_config_set_format(
		struct weston_drm_backend_output_config *ths,
		char const * format) {
	ths->format = strdup(format);
}

void
weston_drm_backend_output_config_set_seat(
		struct weston_drm_backend_output_config *ths,
		char const * seat) {
	ths->seat = strdup(seat);
}

void
weston_drm_backend_output_config_set_modeline(
		struct weston_drm_backend_output_config *ths,
		char const * modeline) {
	ths->modeline = strdup(modeline);
}

/* new load function that need weston_backend_config */
int
weston_drm_backend_load(struct weston_compositor *compositor,
		 int *argc, char **argv, struct weston_config *wc,
		 struct weston_backend_config *wbc)
{
	int (*backend_init)(struct weston_compositor *c,
			    int *argc, char *argv[],
			    struct weston_config *config,
			    struct weston_backend_config *config_base);

	backend_init = weston_load_module("drm-backend.so", "backend_init");
	if (!backend_init)
		return -1;

	return backend_init(compositor, argc, argv, wc, wbc);
}


