#include <gst/gst.h>


int main(int argc, char *argv[]) {
  GstElement *playbin;
  GstBus *bus;
  GstMessage *msg;
  GstStateChangeReturn ret; 
  /* Initialize GStreamer */
  gst_init (&argc, &argv);
   


playbin   = gst_element_factory_make ("playbin2" ,"playbin");


g_object_set (playbin, "uri", "file:/home/pi19404/Downloads/sample.webm", NULL);



 /* Start playing */
  ret = gst_element_set_state (playbin, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (playbin);
    return -1;
  }
   
 /* Add a bus watch, so we get notified when a message arrives */
  bus = gst_element_get_bus (playbin);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
   
  /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (playbin, GST_STATE_NULL);
  gst_object_unref (playbin);
  return 0;
}
