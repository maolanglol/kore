/*
 * Copyright (c) 2014 Joris Vink <joris@coders.se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <kore/kore.h>
#include <kore/http.h>

int		page(struct http_request *);

int
page(struct http_request *req)
{
	u_int16_t		id;
	char			*sid;
	struct kore_buf		*buf;

	/*
	 * Before we are able to obtain any parameters given to
	 * us via the query string we must tell Kore to parse and
	 * validate them.
	 *
	 * NOTE: All parameters MUST be declared in a params {} block
	 * inside the configuration for Kore! Kore will filter out
	 * any parameters not explicitly defined.
	 *
	 * See conf/parameters.conf on how that is done, this is an
	 * important step as without the params block you will never
	 * get any parameters returned from Kore.
	 */
	http_populate_get(req);

	/*
	 * Lets grab the "id" parameter if available. Kore can obtain
	 * parameters in different data types native to C.
	 *
	 * In this scenario, lets grab it both as an actual string and
	 * as an u_int16_t (unsigned short).
	 *
	 * When trying to obtain a parameter as something else then
	 * a string, Kore will automatically check if the value fits
	 * in said data type.
	 *
	 * For example if id is 65536 it won't fit in an u_int16_t
	 * and Kore will return an error when trying to read it as such.
	 */

	buf = kore_buf_alloc(128);

	/* Grab it as a string, we shouldn't free the result in sid. */
	if (http_argument_get_string(req, "id", &sid))
		kore_buf_appendf(buf, "id as a string: '%s'\n", sid);

	/* Grab it as an actual u_int16_t. */
	if (http_argument_get_uint16(req, "id", &id))
		kore_buf_appendf(buf, "id as an u_int16_t: %d\n", id);

	/* Now return the result to the client with a 200 status code. */
	http_response(req, 200, buf->data, buf->offset);
	kore_buf_free(buf);

	return (KORE_RESULT_OK);
}
