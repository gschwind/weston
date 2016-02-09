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

#ifndef SRC_COMPOSITOR_DRM_STATIC_H_
#define SRC_COMPOSITOR_DRM_STATIC_H_

struct weston_drm_backend_output_config {
	struct weston_backend_output_config base;

	/** The pixel format to be used by the output. Valid values are:
	 * - NULL - The format set at backend creation time will be used;
	 * - "xrgb8888";
	 * - "rgb565"
	 * - "xrgb2101010"
	 */
	char *format;
	/** The seat to be used by the output. Set to NULL to use the
	 * default seat. */
	char *seat;
	/** The modeline to be used by the output. Refer to the documentation
	 * of WESTON_DRM_BACKEND_OUTPUT_PREFERRED for details. */
	char *modeline;
};

/** The backend configuration struct.
 *
 * weston_drm_backend_config contains the configuration used by a DRM
 * backend. The backend will take ownership of the weston_backend_config
 * object passed to it on initialization and will free it on destruction. */
struct weston_drm_backend_config {
	struct weston_backend_config base;

	/** The connector id of the output to be initialized. A value of 0 will
	 * enable all available outputs. */
	int connector;
	/** The tty to be used. Set to 0 to use the current tty. */
	int tty;
	/** If true the pixman renderer will be used instead of the OpenGL ES
	 * renderer. */
	bool use_pixman;

	/** reuse the current output mode */
	bool use_current_mode;

	/** The seat to be used for input and output. If NULL the default "seat0"
	 * will be used.
	 * The backend will take ownership of the seat_id pointer and will free
	 * it on backend destruction. */
	char *seat_id;
	/** The pixel format of the framebuffer to be used. Valid values are:
	 * - NULL - The default format ("xrgb8888") will be used;
	 * - "xrgb8888";
	 * - "rgb565"
	 * - "xrgb2101010"
	 * The backend will take ownership of the format pointer and will free
	 * it on backend destruction. */
	char *format;

	/** Callback used to configure the outputs. This function will be called
	 * by the backend when a new DRM output needs to be configured. */
	enum weston_drm_backend_output_mode
		(*configure_output)(struct weston_compositor *compositor,
				    struct weston_drm_backend_config *backend_config,
				    const char *name,
				    struct weston_drm_backend_output_config *output_config);
};


#endif /* SRC_COMPOSITOR_DRM_STATIC_H_ */
