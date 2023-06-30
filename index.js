const node_taglib = require("bindings")("node_taglib");

/**
 * This method extracts the exif metadata from the given file.
 * @param {string} filePath path of the media file.
 * @returns {{
 *      album: string,
 *      artist: string,
 *      genre: string,
 *      title: string,
 *      comment: string,
 *      track: string,
 *      year: string,
 *      bitrate: string,
 *      channels: string,
 *      sampleRate: string,
 *      duration: string
 * }}
 */
function getExifData(filePath) {
  return node_taglib.getExifData(filePath);
}

/**
 * This method update / create the exif data on the given file.
 * @param {string} filePath path of the media file.
 * @param {{
 *      album: string | undefined,
 *      artist: string  | undefined,
 *      genre: string  | undefined,
 *      title: string  | undefined,
 *      comment: string  | undefined,
 *      track: string  | undefined,
 *      year: string  | undefined,
 * }} exifObject exif metadata keys to update.
 * @returns {boolean}
 */
function setExifData(filePath, exifObject) {
  return node_taglib.setExifData(filePath, exifObject);
}

/**
 * This method extracts the cover art image if exists from the given file,
 * and saves it to the destination.
 * @param {string} filePath path of the media file.
 * @param {string} destinationFile destination path to save the cover art.
 * @returns {boolean}
 */
function saveCoverArt(filePath, destinationFile) {
  return node_taglib.saveCoverArt(filePath, destinationFile);
}

/**
 * This method returns the cover art image data bytes (uint8array)
 * @param {string} filePath path of the media file.
 * @param {(buffer: Uint8Array) => void} cb callback function which will receive the buffer data.
 * @returns {void}
 */
function getCoverArt(filePath, cb) {
  node_taglib.getCoverArt(filePath, cb);
}

/**
 * This method will set the new cover art to the given media file.
 * @param {string} filePath path of the media file.
 * @param {"image/png" | "image/jpeg"} mimeType mime type of the new cover art.
 * @param {string | Buffer} newCoverArt new cover art image file path or content (buffer).
 * @returns {boolean}
 */
function setNewCoverArt(filePath, mimeType, newCoverArt) {
  return node_taglib.setNewCoverArt(filePath, mimeType, newCoverArt);
}

const NodeTagLib = {
  getExifData,
  setExifData,
  saveCoverArt,
  getCoverArt,
  setNewCoverArt
};

module.exports = NodeTagLib;
