===========
Basic usage
===========

Headers
*******

.. code-block:: c++

   #include <flom/flom.hpp>

All required headers are included in :code:`flom.hpp`


Load / Dump the motion
**********************

Loading and dumping is easy:

.. code-block:: c++

   int main() {
     auto motion = flom::Motion::load("file.fom");

     // ... Edit loaded motion

     motion.dump("out.fom");
   }

We recommend to use :code:`.fom` as a file extension.

You can use :code:`Motion::load_json` or :code:`Motion::dump_json`
if you like json


Obtain a frame
**************

Use :code:`Motion::frame_at` to obtain a frame at arbitrary time point. For example:

.. code-block:: c++

   // Assume motion is an instance of flom::Motion
   auto frame = motion.frame_at(1.5);

Here, :code:`frame` is a frame at 1.5 second since the motion is started.


Iterate over frames
*******************

.. code-block:: c++

   for (auto&& [t, frame] : motion.frames(10)) {
      // Do something with frame
   }

In this way, frames are iterated in 10fps(not actual time, but the time in the motion!).
Also :code:`t` holds the time of current frame.
