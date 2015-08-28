ofxEyeTribe
===========

An openFrameworks wrapper for [The Eye Tribe](https://theeyetribe.com/) SDK 

<a href="https://vimeo.com/115662398">
![image](./image.png)
</a>

[demo movie](https://vimeo.com/115662398)


#Usage

To add ofxEyeTrive to your project:

- Drag the ofxEyeTribe addon folder into your project.
- Add the location of the libraries (`addons/ofxEyeTribe/libs/EyeTribeCppClient/lib/osx`) to the Library Search Paths Build Setting in your Xcode project.
- Add the libraries in above location to the Link Binary With Libraries Build Phases in your Xcode project.

The included example demonstrates these steps.

#Change Log

You can checkout for each version. See Tags in master branch.  

**Ver. 0.3**

- Supported Visual Studio 2015 x64 (Thank you [Matt](https://github.com/mattfelsen))

**Ver. 0.2**

- Imprementations were divided from ofxEyeTrive.h, (if you created a your original project include version 0.1 ofxEyeTribe, please add `ofxEyeTribe/src/ofxEyeTribe.cpp` to your project.)

**Ver. 0.1**



#Licensing

The Eye Tribe SDK is licensed under the BSD-style license found in the LICENSE file.