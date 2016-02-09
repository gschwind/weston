/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2015 Giulio Camuffo
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

#ifndef WESTON_COMPOSITOR_DRM_H
#define WESTON_COMPOSITOR_DRM_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "compositor.h"

struct weston_drm_backend_config;
struct weston_drm_backend_output_config;

enum weston_drm_backend_output_mode {
	/** The output is disabled */
	WESTON_DRM_BACKEND_OUTPUT_OFF,
	/** The output will use the current active mode */
	WESTON_DRM_BACKEND_OUTPUT_CURRENT,
	/** The output will use the preferred mode. A modeline can be provided
	 * by setting weston_backend_output_config::modeline in the form of
	 * "WIDTHxHEIGHT" or in the form of an explicit modeline calculated
	 * using e.g. the cvt tool. If a valid modeline is supplied it will be
	 * used, if invalid or NULL the preferred available mode will be used. */
	WESTON_DRM_BACKEND_OUTPUT_PREFERRED,
};

/** Callback used to configure the outputs. This function will be called
 * by the backend when a new DRM output needs to be configured. */
typedef enum weston_drm_backend_output_mode
	(*weston_drm_backend_configure_output_func)(struct weston_compositor *compositor,
			    struct weston_drm_backend_config *backend_config,
			    const char *name,
			    struct weston_drm_backend_output_config *output_config);

/** Create a new default drm backend configuration */
struct weston_drm_backend_config *
weston_drm_backend_config_create(void);

/** The connector id of the output to be initialized.
 * default: enable all available outputs. */
void
weston_drm_backend_config_set_connector(
		struct weston_drm_backend_config * ths,
		int connector);

/** The tty to be used. default: use the current tty. */
void
weston_drm_backend_config_set_tty(
		struct weston_drm_backend_config * ths,
		int tty);

/** If true the pixman renderer will be used instead of the OpenGL ES
 * renderer. */
void weston_drm_backend_config_set_use_pixman(
		struct weston_drm_backend_config * ths,
		bool x);

/** reuse the current output mode */
void
weston_drm_backend_config_set_use_current_mode(
		struct weston_drm_backend_config * ths,
		bool x);

bool
weston_drm_backend_config_get_use_current_mode(
		struct weston_drm_backend_config * ths);

/** The seat to be used for input and output. If NULL the default "seat0"
 * will be used.
 * The backend will take ownership of the seat_id pointer and will free
 * it on backend destruction. */
void
weston_drm_backend_config_set_seat_id(
		struct weston_drm_backend_config * ths,
		char const * seat_id);

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
		char const * format);

/** Callback used to configure the outputs. This function will be called
 * by the backend when a new DRM output needs to be configured. */
void
weston_drm_backend_config_set_configure_output_func(
		struct weston_drm_backend_config * ths,
		weston_drm_backend_configure_output_func func);

struct weston_backend_config *
weston_drm_backend_config_get_weston_backend_config(
		struct weston_drm_backend_config *ths);

void
weston_drm_backend_output_config_set_scale(
		struct weston_drm_backend_output_config *ths,
		uint32_t scale);

void
weston_drm_backend_output_config_set_transform(
		struct weston_drm_backend_output_config *ths,
		uint32_t transform);

void
weston_drm_backend_output_config_set_format(
		struct weston_drm_backend_output_config *ths,
		char const * format);

void
weston_drm_backend_output_config_set_seat(
		struct weston_drm_backend_output_config *ths,
		char const * seat);

void
weston_drm_backend_output_config_set_modeline(
		struct weston_drm_backend_output_config *ths,
		char const * modeline);

int
weston_drm_backend_load(struct weston_compositor *compositor,
		 int *argc, char **argv, struct weston_config *wc,
		 struct weston_backend_config *wbc);

#ifdef  __cplusplus
}
#endif

#endif
