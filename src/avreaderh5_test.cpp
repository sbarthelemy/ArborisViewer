
#ifndef __ARBORISVIEWERTEST__
#define __ARBORISVIEWERTEST__

#include <cppunit/extensions/HelperMacros.h>
#include "avreaderh5.h"
#include <cmath>

using namespace av;

class H5RandomReaderTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( H5RandomReaderTest );
    CPPUNIT_TEST( testFirstFrameEquality );
    CPPUNIT_TEST_SUITE_END();

private:
    H5RandomReader *reader;

public:
    void setUp() {
        reader = new H5RandomReader("../data/elementary.h5", "/");
    }

    void tearDown() {
        delete reader;
    }

    void testFirstFrameEquality() {
        bool isEqual = true;
        FrameData fd( reader->getFrame(0) );
        Matrix m(000., 001., 002., 003.,
                 010., 011., 012., 013.,
                 020., 021., 022., 023.,
                 030., 031., 032., 033.);

        for (int i=0; i<16; i++) {
            if ( std::abs( (fd.matrices["matrix"])[i] - m[i] ) > 1e-8 ) {
                isEqual = false;
                break;
            }
        }
        CPPUNIT_ASSERT( isEqual );
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( H5RandomReaderTest );

#endif

