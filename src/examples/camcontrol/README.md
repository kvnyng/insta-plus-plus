# Camera Controller
A high level class for controlling an inta360 camera.

## Getting started
To connect to the camera, create an instance of the `CameraController()` class. Upon initialization, the camera will automatically be connected to and setup based on internal functions.

## Streaming
To begin streaming, simply call the `.startStream()` function. This will feed a `StreamDelegate` to the camera, which controls what the camera will do when it receives video, audio, or IMU data. To close the stream, call `.stopStream()`.

## Ending Connection
Once your done with the camera, call `.close()` to terminate the connection. This is to prevent memory leaks and re-free system resources.