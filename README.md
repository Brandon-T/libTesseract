libTesseract
============

A build of the tesseract module for Pascal, C and C++

============

Libraries to link to (in order):

  - user32
  - tesseract
  - lept
  - libz
  - liblzma
  - libpng
  - libjpeg
  - ws2_32
  
  
  
To build the above libraries, use the following commands (in order):


Building ZLIB: http://zlib.net/zlib-1.2.8.tar.gz

    BINARY_PATH=/usr/local/bin \
    INCLUDE_PATH=/usr/local/include \
    LIBRARY_PATH=/usr/local/lib \
    make -f win32/Makefile.gcc install


Building LZMA: http://tukaani.org/xz/xz-5.0.5.tar.gz

    ./configure
    make install


Building LIBPNG: http://prdownloads.sourceforge.net/libpng/libpng-1.6.10.tar.gz?download

    cp scripts/makefile.gcc .

    DESTDIR=/usr/local \
    ZLIBLIB=/usr/local/lib \
    ZLIBINC=/usr/local/include \
    make -f makefile.gcc

    cp png.h pngconf.h /usr/local/include
    cp libpng.a libpng.dll.a /usr/local/lib


BUILDING LIBJPEG: http://www.ijg.org/files/jpegsrc.v8d.tar.gz
    
    ./configure
    make install


BUILDING LEPTONICA: http://www.leptonica.com/source/leptonica-1.70.tar.gz

    ./autobuild
    ./configure --no-programs
    make install


BUILDING TESSERACT-OCR: https://code.google.com/p/tesseract-ocr/downloads/detail?name=tesseract-ocr-3.02.02.tar.gz&can=2&q=

    ./autogen.sh
    CPPFLAGS="-I/usr/local/include" LDFLAGS="-L/usr/local/lib" ./configure --disable-tessdata-prefix
    make install