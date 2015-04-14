#pragma once

#include "exceptions.h"
#include <functional>

namespace cv_pipeline {

// load an image
inline
cv::Mat load(char const * const pathname)
{
    return cv::imread(pathname);
}

// load an image
inline
cv::Mat load(std::string const &pathname)
{
    return cv::imread(pathname);
}

// save an image
inline
cv::Mat save(char const * const pathname, cv::Mat const &image)
{
    return cv::imwrite(pathname, image)? image : cv::Mat();
}

inline
cv::Mat grey(cv::Mat const &img)
{
    cv::Mat image;
    cvtColor(img, image, cv::COLOR_BGR2GRAY);
    cvtColor(image, image, cv::COLOR_GRAY2BGR);
    return image;
}

inline
cv::Mat mirror(cv::Mat const &img)
{
    cv::Mat image;
    flip(img, image, 1);
    return image;
}

// apply a function to an image
inline
cv::Mat operator|(cv::Mat const &left, cv::Mat(*right)(cv::Mat const &))
{
    return right(left);
}

// apply a function to an image - enables std::bind() bound parameters
inline
cv::Mat operator|(cv::Mat const &left, std::function<cv::Mat(cv::Mat const &)> right)
{
    return right(left);
}

typedef enum { noverify=false, verify=true } verify_result;

// load an image with optional verification.
// whether or not to verify is compulsory -- verify or noverify must
// go between a load and any subsequent manipulations through the
// pipeline interface
inline
cv::Mat operator|(char const * const pathname, verify_result verify)
{
    cv::Mat image = load(pathname);
    if (verify  &&  image.empty())
        throw exceptions::image_not_found(pathname);
    return image;
}

inline
cv::Mat operator|(std::string const pathname, verify_result verify)
{
    cv::Mat image = load(pathname);
    if (verify  &&  image.empty())
        throw exceptions::image_not_found(pathname.c_str());
    return image;
}

// verify an image is not empty
inline
cv::Mat operator|(cv::Mat const &image, verify_result verify)
{
    if (verify  &&  image.empty())
        throw exceptions::bad_image();
    return image;
}


}   // namespace cv_pipeline