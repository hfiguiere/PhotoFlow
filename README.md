PhotoFlow
=========

The aim of the project is to implement a fully non-destructive photo retouching program that provides a complete workflow from RAW image development to high-quality printing.

Here is the general feature (whish)list:

- Fully non-destructive, layer-based photo editing workflow with realtime preview of the final image
- Support for 8-bits and 16-bits integer as well as 32-bits and 64-bits floating point precision, selectable at runtime and on a per-image basis
- Plugin-based architecture: most of the tools are implemented as separate modules that are loaded at runtime
- Tile-based image caching to limit the memory footprint and allow loading images of arbitrary size
- Fully color managed workflow: user-defined input, work and output profiles, soft-proofing, etc...
- Support for layer grouping and layer masks
- Support for common photo editing tools: levels, curves, brightness-contrast control, blurring, sharpening, cropping, resizing, colorspace conversions, etc..., all implemented in the form of image layers
- User-friendly interface to develop new tools and image filters as external plugins

Downloading and compiling
=========================

The pixel rendering angine of PhotoFlow is based on VIPS (http://www.vips.ecs.soton.ac.uk). You have to install a recent version of VIPS in order to compile PhotoFlow.