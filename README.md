>**Warning**
>Development Phase.
>macOS & linux support only.
> adding support for windows as well.

### Node.js binding for taglib. ``Still in development phase``.

##### TagLib is a ``C++`` library for reading and editing the metadata of several popular audio formats. To know more click [here](https://taglib.org/).

### How to use

```js
const NodeTagLib = require("node-taglib");

const exifData = NodeTagLib.getExifData("path_to_audio.mp3");

console.log(exifData);
/*
{
    album: "string",
    artist: "string",
    title: "string",
    genre: "string",
    comment: "string",
    track: "string",
    year: "string",
    bitrate: "string",,
    channels: "string"
    sampleRate: "string",
    duration: "string"
}
*/

------------
------------
------------
// How to update the exif data.
// You can omit the key which you don't want to update.
const success = NodeTagLib.setExifData("path_to_audio.mp3", {
    album: "my album",
    artist: "my artist"
});

if(success)
    console.log("exif data updated successfully...");

------------
------------
------------
//--------------------------------------------//
// EXTRACT COVER ART (IMAGE) FROM MUSIC FILE.
//--------------------------------------------//
// This will create a new file & save cover image to the destination.
const success = NodeTagLib.saveCoverArt("path_to_audio.mp3", "new_cover_art.jpeg");

if(success)
    console.log("conver image extracted...");

// Get the cover image data (UInt8Array)
NodeTagLib.getCoverArt("path_to_audio.mp3", (buffer) => {
    console.log(buffer);
});
//--------------------------------------------//
// SET NEW IMAGE AS COVER ART TO MUSIC FILE.
//--------------------------------------------//
// Set new cover art image to music file
// 1st argument is music file to which new cover image will be written.
// 2nd argument is mime-type- image/jpeg or image/png
// 3rd argument is the image file.
NodeTagLib.setNewCoverArt("path_to_audio.mp3", "image/jpeg", "path_to_image.jpg");
```

``bitrate`` is in ``kb/s``
``sampleRate`` is in ``Hz``
``duration`` is in ``seconds``

### Available Methods
1. ``getExifData`` accepts music ``file-path`` as argument & ``returns`` an object containing keys-
``album, artist, title, genre, comment, track, year, bitrate,, channels sampleRate, duration``.

2. ``setExifData`` accepts an object containing exif meta-data keys to update & return true / false. Valid keys- ``album, artist, title, genre, comment, track, year``.

3. ``saveCoverArt`` accepts two arguments, 1st the music ``file-path`` & 2nd one is the ``destination`` it will write the image data from music file (if present) to the given destination, & return true / false.

4. ``getCoverArt`` accepts two argument 1st the music ``file-path`` & 2nd as a callback, the given callback will be called with the image data buffer (uint8-array).

5. ``setNewCoverArt`` this method is used to update the the cover art image of music file. It accepts 3 arguments, 1st the music ``file-path``, 2nd one is ``mime-type`` (image/png, image/jpeg) & the 3rd one is the ``image-path`` which we want to set as cover image. It returns true / false.

#### How to build the project
just run ``npm run build``.

### Bindings to be written

- [x] Get exif data.
- [x] Set / Update exif data.
- [x] Save cover image.
- [x] Get cover image data (bytes).
- [x] Set cover image.
- [ ] many more features coming soon...
