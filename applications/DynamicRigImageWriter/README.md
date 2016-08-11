ImageWriter
======

This application serves as a simple interface to PathViz for image generation.

Given a `cameras.xml` and CSV pose file, images are rendered and saved to the
specified output directory as `capture_<camera_id>_<frame_id>.png`. All images
are saved as single-channel grayscale images.

### Usage

    image_writer rig_file pose_file output_dir [rand_seed]

#### Example

    image_writer /Users/arpg/Documents/cameras.xml /Users/arpg/Documents/poses.csv /Users/arpg/Desktop/images 571

#### Arguments

- __`rig_file`__: Absolute file path to `cameras.xml` file describing the camera rig used to render the generated scene. One ore more cameras can be specified here. Only the `calibu_fu_fv_u0_v0` camera model is supported.

- __`pose_file`__: Absolute file path to CSV pose file. The CSV file should contain 7 numerical values per row in the following format:

        timestamp, x, y, z, roll, pitch, yaw

    Note `roll`, `pitch`, and `yaw` are in radians.

- __`output_dir`__: Absolute file path to output image folder

- __`rand_seed`__ (optional): unsigned integer to seed the random-number-generator used when randomly generating the scene. So if you wish to generate the same scene for two different camera rigs you can manually specify the same seed for both renders. (Note that the path file must remain unchanged)
