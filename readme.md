OpenCV Pipeline
===============

A C++11 pipeline interface to OpenCV 2.4. See `develop` branch for current implementation.

Motivation
----------

Why write this:

    cv::Mat img = cv::imread("..\\..\\..\\..\\test data\\images\\Mona_Lisa_headcrop.jpg");
    if (img.empty())
        throw std::runtime_error("file not found");

    cvtColor(img, img, cv::COLOR_BGR2GRAY);
    cvtColor(img, img, cv::COLOR_GRAY2BGR);
    cv::flip(img, img, 1);

when you can write this:

    using namespace cv_pipeline;
    cv::Mat img = "monalisa.jpg" | verify | grey | mirror;

Principles
==========

* Lightweight
* Safe
* Efficient

Lightweight
-----------

*OpenCV Pipeline* is designed to be lightweight and expression (somewhat functional). To change an image to grey scale, reflect it horizontally and save it back out, simply write:

    using namespace opencv_pipeline;
    "colour.png" | verify | grey | verify | mirror | std::bind(save, "result.png", std::placeholders::_1);

That's all there is to it. No fuss, no variables, just a simple pipeline of actions. If you want a copy of the result for further processing, assign the expression to a variable:

    using namespace opencv_pipeline;
    auto grey_mirror = "colour.png" | verify | grey | verify | mirror | std::bind(save, "result.png", std::placeholders::_1);

Safe
----

Inline verification is always available. Added `verify` at any point in the pipeline and if the previous step produced an invalid result (empty `Mat`), then an exception is thrown, with information where it is available.

It is mandatory to be explicit about error checking when loading an image within a pipeline. If you really want to avoid it, you can use `noverify`, but this is generally discouraged:

    auto image = "colour.png" | noverify | grey;

If the image load fails, and `noverify` is specified, then an empty image is passed to the next function in the pipeline. In this case, `grey` which calls OpenCV's `cvtColor` which will fail. If you are in exception free, then split the pipeline and use `noverify`:

		auto image = "colour.png" | noverify;
		if (!image.empty())
			image = image | grey;

Efficient
---------

Some efficiency is compromised in the implementation with the hope that the compiler will be able to optimise the resulting code. OpenCV's reference counted `Mat` structures are a pain for optimisation, and return-by-value which should be a move operation isn't because of the ref-counted design.
