#ifndef JPEGDEC_H_
#define JPEGDEC_H_

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <jpeglib.h>

namespace jpegutil {

/**
 * JPEG decoder.
 */
class JpegDec {
public:
    JpegDec();
    virtual ~JpegDec();

    JpegDec(const JpegDec&) = delete;
    void operator=(const JpegDec&) = delete;

    /**
     * Sets the color space of the images to be worked with.
     *
     * @param colorSpace  the color space. Can be: JCS_GRAYSCALE, JCS_RGB, JCS_YCbCr, or JCS_CMYK.
     */
    void setColorSpace(J_COLOR_SPACE colorSpace);

    /**
     * Gets the current color space.
     *
     * @return  the color space identifier
     */
    J_COLOR_SPACE getColorSpace() const;

    /**
     * Decodes a JPEG image from a memory buffer.
     *
     * @param[in]  buffer  the pointer to the buffer with encoded image
     * @param[in]  len     the size of the buffer
     * @param[out] img     the decoded raw image. The memory is allocated inside the function.
     * @param[out] width   the width of the decoded image
     * @param[out] height  the height of the decoded image
     * @return             true if success, false if failed
     */
    bool decode(uint8_t* buffer, size_t len, uint8_t*& img, int& width, int& height);

    /**
     * Reads and decodes a JPEG image through file descriptor.
     *
     * @param[in]  file    the file descriptor
     * @param[out] img     the decoded raw image. The memory is allocated inside the function.
     * @param[out] width   the width of the decoded image
     * @param[out] height  the height of the decoded image
     * @return             true if success, false if failed
     */
    bool decode(FILE* file, uint8_t*& img, int& width, int& height);

    /**
     * Reads and decodes a JPEG file on disk.
     *
     * @param[in]  path    the path to the file
     * @param[out] img     the decoded raw image. The memory is allocated inside the function.
     * @param[out] width   the width of the decoded image
     * @param[out] height  the height of the decoded image
     * @return             true if success, false if failed
     */
    bool decode(const char* path, uint8_t*& img, int& width, int& height);

private:
    bool decode(uint8_t*& img, int& width, int& height);

    jpeg_decompress_struct decInfo;
    jpeg_error_mgr errMgr;

    J_COLOR_SPACE colorSpace = JCS_RGB;
};

}

#endif // JPEGDEC_H_
