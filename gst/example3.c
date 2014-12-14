#include <gst/gst.h>



static void
on_pad_added (GstElement * element, GstPad * pad, gpointer data)
{
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the decoder sink pad */
  g_print ("Dynamic pad created, linking demuxer/decoder\n");

  sinkpad = gst_element_get_static_pad (decoder, "sink");

  gst_pad_link (pad, sinkpad);

  gst_object_unref (sinkpad);
}
   
int main(int argc, char *argv[]) {
  GstElement *pipeline,*source,*sink,*decoder,*filter,*demux,*queue;
  GstBus *bus;
  GstMessage *msg;
   
  /* Initialize GStreamer */
  gst_init (&argc, &argv);
   


source   = gst_element_make_from_uri (GST_URI_SRC,"file:/home/pi19404/Downloads/sample.webm" ,NULL);
filter = gst_element_factory_make("ffmpegcolorspace","colorspace");
sink = gst_element_factory_make ("autovideosink", "sink");
demux=gst_element_factory_make ("matroskademux", "demux");
queue=gst_element_factory_make ("queue", "q1");
decoder=gst_element_factory_make ("vp8dec", "decoder");

//g_object_set (G_OBJECT (source), "location", "/home/pi19404/Downloads/sample.webm", NULL);



/* Create the empty pipeline */
pipeline = gst_pipeline_new ("test-pipeline");


/* Adding Elements to List */
gst_bin_add_many (GST_BIN (pipeline), source,demux,queue,decoder,filter,sink, NULL);



if (gst_element_link (source, demux) !=TRUE || gst_element_link_many (decoder, filter,sink,NULL) !=TRUE) {
  g_printerr ("Elements could not be linked.\n");
  gst_object_unref (pipeline);
  return -1;
}

g_signal_connect(G_OBJECT(demux),"pad-added",G_CALLBACK(on_pad_added),decoder);

   
/* Start playing */
int ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
if (ret == GST_STATE_CHANGE_FAILURE) {
  g_printerr ("Unable to set the pipeline to the playing state.\n");
  gst_object_unref (pipeline);
  return -1;
}
   
  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
   
  /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}
