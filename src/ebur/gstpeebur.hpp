#ifndef _GST_PEEBUR_H_
#define _GST_PEEBUR_H_

#include <ebur128.h>
#include <gst/audio/gstaudiofilter.h>
#include <mutex>

G_BEGIN_DECLS

#define GST_TYPE_PEEBUR (gst_peebur_get_type())
#define GST_PEEBUR(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_PEEBUR, GstPeebur))
#define GST_PEEBUR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_PEEBUR, GstPeeburClass))
#define GST_IS_PEEBUR(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_PEEBUR))
#define GST_IS_PEEBUR_CLASS(obj) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_PEEBUR))

typedef struct _GstPeebur GstPeebur;
typedef struct _GstPeeburClass GstPeeburClass;

struct _GstPeebur {
    GstAudioFilter base_peebur;

    /* properties */

    bool post_messages; /* whether or not to post messages */
    guint64 interval;   /* how many nanoseconds between emits */

    /* < private > */

    bool ready;
    GstAdapter* adapter;
    int bpf;  // bytes per frame : channels * bps
    double loudness, peak_L, peak_R;
    int rate;            /*sampling rate*/
    int interval_frames; /* after how many frame to sent a message */
    ebur128_state* ebur_state;

    std::mutex lock_guard_ebu;
};

struct _GstPeeburClass {
    GstAudioFilterClass base_peebur_class;
};

GType gst_peebur_get_type(void);

G_END_DECLS

#endif
