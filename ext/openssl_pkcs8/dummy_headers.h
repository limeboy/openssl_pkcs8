/**
 * Dummy headers for openssl ruby functions that are not available at link time
 */
#define OSSL_Check_Kind(obj, klass) do {\
   if (!rb_obj_is_kind_of((obj), (klass))) {\
     ossl_raise(rb_eTypeError, "wrong argument (%s)! (Expected kind of %s)",\
                rb_obj_classname(obj), rb_class2name(klass));\
   }\
} while (0)


VALUE dOSSL;

static VALUE
ossl_make_error(VALUE exc, const char *fmt, va_list args)
{
    char buf[BUFSIZ];
    const char *msg;
    long e;
    int len = 0;

#ifdef HAVE_ERR_PEEK_LAST_ERROR
    e = ERR_peek_last_error();
#else
    e = ERR_peek_error();
#endif
    if (fmt) {
        len = vsnprintf(buf, BUFSIZ, fmt, args);
    }
    if (len < BUFSIZ && e) {
        if (dOSSL == Qtrue) /* FULL INFO */
            msg = ERR_error_string(e, NULL);
        else
            msg = ERR_reason_error_string(e);
        len += snprintf(buf+len, BUFSIZ-len, "%s%s", (len ? ": " : ""), msg);
    }
    if (dOSSL == Qtrue){ /* show all errors on the stack */
        while ((e = ERR_get_error()) != 0){
            rb_warn("error on stack: %s", ERR_error_string(e, NULL));
        }
    }
    ERR_clear_error();

    if(len > BUFSIZ) len = rb_long2int(strlen(buf));
    return rb_exc_new(exc, buf, len);
}

void ossl_raise(VALUE exc, const char *fmt, ...)
{
    va_list args;
    VALUE err;
    va_start(args, fmt);
    err = ossl_make_error(exc, fmt, args);
    va_end(args);
    rb_exc_raise(err);
}