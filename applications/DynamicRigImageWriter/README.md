DynamicRigImageWriter
======

This application serves as a simple interface to PathViz for image generation
with a dynamic camera rig, allowing a rig intrinsics and extrinsics to change
over time.

Given a list of `cameras.xml` files, interpolation values, and a CSV pose file,
images are rendered with the computed camera rig and saved to the specified
output directory as `capture_<camera_id>_<frame_id>.png`. All images are saved
as single-channel grayscale images.

### Usage

    dynamic_rig_image_writer rig_list_file interp_file pose_file output_dir [rand_seed]

#### Example

    dynamic_rig_image_writer rigs.txt interp.txt poses.csv output_images/ 571

#### Arguments

- __`rig_list_file`__: Absolute file path to a plain text file listing each path
  to a `cameras.xml` file to be used to render the generated scene. Each file
  must be placed on a separate line. One or more cameras can be specified in each
  rig file, but the number of cameras and their resolution must be the same
  across all files. Only the `calibu_fu_fv_u0_v0` camera model is supported.

- __`interp_file`__: A plain text file with the values for interpolating between
  rigs. These are floating-point values that can range between `[0, N-1]`, where
  `N` is the number of rig files specified in `rig_list_file`. As an example, if
  the value 1.2 is given, it will result in a camera rig that is a combination
  of the second and third rig file with a weight of 0.8 and 0.2 respectively.
  Each value must be placed on a separate line and total number of lines must
  match the number of poses listed in `pose_file`.

- __`pose_file`__: Absolute file path to CSV pose file. The CSV file should
  contain 7 numerical values per row in the following format:

        timestamp, x, y, z, roll, pitch, yaw

    Note `roll`, `pitch`, and `yaw` are in radians.

- __`output_dir`__: Absolute file path to output image folder

- __`rand_seed`__ (optional): unsigned integer to seed the
  random-number-generator used when randomly generating the scene. So if you
  wish to generate the same scene for two different camera rigs you can manually
  specify the same seed for both renders. (Note that the path file must remain
  unchanged)
