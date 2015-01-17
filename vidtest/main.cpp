// (c) Tord Wessman 2013
// Feel free to do what you like with code.
//
// This simple example demonstrates how to use the tee elements to
// display two xvimagesink windows containing one web-cam input (v4l2src).
//


#include <cstdio>
#include <gst/gst.h>
//#include <QtGStreamer/QGlib/Error>
//#include <QtGStreamer/QGlib/Connect>
//#include <QtGStreamer/QGst/Init>
//#include <QtGStreamer/QGst/Bus>
//#include <QtGStreamer/QGst/Pipeline>
//#include <QtGStreamer/QGst/Parse>
//#include <QtGStreamer/QGst/Message>
//#include <QtGStreamer/QGst/Utils/ApplicationSink>
//#include <QtGStreamer/QGst/Utils/ApplicationSource>

static GMainLoop *loop;

static GstElement *bin,  // the containing all the elements
  *pipeline,
  *src,
  *csp,
  *tee,
  *q1,*q2,
  *testsink,
  *sink;

static GstBus *bus; //the bus element te transport messages from/to the pipeline
QGst::Element videoSink = QGst::ElementFactory::make("qtglvideosink");
static gboolean bus_call(GstBus *bus, GstMessage *msg, void *user_data);

int init() {
 gst_init (NULL, NULL);

 GstCaps *caps;

 /* create the main loop */
 loop = g_main_loop_new(NULL, FALSE);

 pipeline = gst_pipeline_new ("video_pipeline");

 /* create the bus for the pipeline */
 bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

 /* add the bus handler method */
 gst_bus_add_watch(bus, bus_call, NULL);

 gst_object_unref(bus);

 bin = gst_bin_new ("video_bin");

 //initializing elements
 src = gst_element_factory_make ("v4l2src", "src");
 sink = gst_element_factory_make ("xvimagesink", "xvimagesinkONE");
 testsink = gst_element_factory_make ("xvimagesink", "testsinkTWO");
 csp = gst_element_factory_make("videoconvert", "csp");
 tee = gst_element_factory_make ("tee", "videotee");
 q1 = gst_element_factory_make ("queue", "qone");
 q2 = gst_element_factory_make ("queue", "qtwo");

 if (src == NULL || sink == NULL  || testsink == NULL) {
  g_critical ("Unable to create src/sink elements.");
  return 0;
 } else  if (csp == NULL) {
  g_critical ("Unable to create csp");
  return 0;
 } else if (!q1 && !q2 && !tee) {
  g_critical ("Unable to create other elements");
  return 0;
 }

 /* Add the elements to the pipeline prior to linking them */

 gst_bin_add_many(GST_BIN(pipeline), src, csp, tee, q1, sink, q2, testsink, NULL);

 /* Specify caps for the csp-filter (modify this if your hardware requires) */

 caps = gst_caps_new_simple("video/x-raw",
   "width", G_TYPE_INT, 640,
   "height", G_TYPE_INT, 480,
   NULL);

 /* Link the camera source and csp filter using capabilities
  * specified */

 if(!gst_element_link_many(src, csp, NULL))
 {
  gst_object_unref (pipeline);
  g_critical ("Unable to link src to csp ");
  return 0;
 }

 /* link the tee element */

 if(!gst_element_link_filtered(csp, tee, caps))
 {
  gst_object_unref (pipeline);
  g_critical ("Unable to link csp to tee. check your caps.");
  return 0;
 }

 /* Link the first sink */
 if(!gst_element_link_many(q1, sink, NULL))
 {
  gst_object_unref (pipeline);
  g_critical ("Unable to link csp->tee->queue->sink for the queue 1");
  return 0;
 }



 /* Link the second sink */
 if(!gst_element_link_many(q2, testsink, NULL))
 {
  gst_object_unref (pipeline);
  g_critical ("Unable to link csp->tee->queue->sink for the queue 2.");
  return 0;
 }

 GstPadTemplate *tee_src_pad_template;
 GstPad *tee_q1_pad, *tee_q2_pad;
   GstPad *q1_pad, *q2_pad;

 /* Manually link the Tee, which has "Request" pads */
 if ( !(tee_src_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS (tee), "src_%u"))) {
  gst_object_unref (pipeline);
  g_critical ("Unable to get pad template");
  return 0;
 }

 /* Obtaining request pads for the tee elements*/
 tee_q1_pad = gst_element_request_pad (tee, tee_src_pad_template, NULL, NULL);
 g_print ("Obtained request pad %s for q1 branch.\n", gst_pad_get_name (tee_q1_pad));
 q1_pad = gst_element_get_static_pad (q1, "sink");

 tee_q2_pad = gst_element_request_pad (tee, tee_src_pad_template, NULL, NULL);
 g_print ("Obtained request pad %s for q2 branch.\n", gst_pad_get_name (tee_q2_pad));
 q2_pad = gst_element_get_static_pad (q2, "sink");

 /* Link the tee to the queue 1 */
 if (gst_pad_link (tee_q1_pad, q1_pad) != GST_PAD_LINK_OK ){

  g_critical ("Tee for q1 could not be linked.\n");
  gst_object_unref (pipeline);
  return 0;

 }

 /* Link the tee to the queue 2 */
 if (gst_pad_link (tee_q2_pad, q2_pad) != GST_PAD_LINK_OK) {

  g_critical ("Tee for q2 could not be linked.\n");
  gst_object_unref (pipeline);
  return 0;
 }

 gst_object_unref (q1_pad);
 gst_object_unref (q2_pad);

 return 1;

}


void start() {


 gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);

 g_main_loop_run(loop);

 gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
}


void stop() {
 g_main_loop_quit(loop);
 gst_object_unref(GST_OBJECT(pipeline));
 g_main_loop_unref (loop);
}


static gboolean bus_call(GstBus *bus, GstMessage *msg, void *user_data)
{

 switch (GST_MESSAGE_TYPE(msg)) {
 case GST_MESSAGE_EOS: {

  g_main_loop_quit(loop);
  break;
 }
 case GST_MESSAGE_ERROR: {
  GError *err;
  gst_message_parse_error(msg, &err, NULL);
  //report error
  printf ("ERROR: %s", err->message);
  g_error_free(err);

  g_main_loop_quit(loop);

  break;
 }
 case GST_MESSAGE_APPLICATION: {

  const GstStructure *str;
  str = gst_message_get_structure (msg);
   if (gst_structure_has_name(str,"turn_off"))
   {
    g_main_loop_quit(loop);
   }

  break;
 }
 default:

  break;
 }
  if (msg->type == GST_MESSAGE_STATE_CHANGED ) {
   GstState old, news, pending;
        gst_message_parse_state_changed (msg, &old, &news, &pending);
   printf ("State changed. Old: %i New: %i Pending: %i.\n", old, news, pending);
  } else {
   printf("info: %i %s type: %i\n", (int)(msg->timestamp), GST_MESSAGE_TYPE_NAME (msg), msg->type);
  }

 return true;
}

int main (int argc, char** argv) {

 if (init()) {
  start ();
  stop();
 }
 else {
  printf ("unable to initialize");
  return -1;
 }

 return 0;
}
