#ifndef SECTIONREADER_H
#define SECTIONREADER_H

// #include "lazyFileReader.h"
// #include "sdaiApplication_instance.h"
#include <fstream>
#include "lazyTypes.h"

class SDAI_Application_instance;
class lazyFileReader;
class ErrorDescriptor;

class sectionReader {
protected:
    //protected data members
    lazyFileReader * _lazyFile;
    std::ifstream & _file;

    std::streampos _sectionStart,  ///< the start of this section as reported by tellg()
                   _sectionEnd;    ///< the end of this section as reported by tellg()
    unsigned long _totalInstances;

    ErrorDescriptor * _error;
    sectionID _sectionID;
    fileID _fileID;

    // protected member functions

    sectionReader( lazyFileReader * parent, std::ifstream & file, std::streampos start, sectionID sid );

    /** Find a string, ignoring occurrences in comments or Part 21 strings (i.e. 'string with \S\' control directive' )
     * \param str string to find
     * \param semicolon if true, 'str' must be followed by a semicolon, possibly preceded by whitespace.
     * \returns the position of the end of the found string
     */
    std::streampos findNormalString( const std::string& str, bool semicolon = false );

    /** Get a keyword ending with one of delimiters.
     */
    const char * getDelimitedKeyword( const char * delimiters );

    /** Seek to the end of the current instance */
    std::streampos seekInstanceEnd( std::set< instanceID > * refs );

    /// operator>> is very slow?!
    inline void skipWS() {
        while( isspace( _file.peek() ) && _file.good() ) {
            _file.ignore( 1 );
        }
    }

public:
    SDAI_Application_instance * getRealInstance( lazyInstanceLoc * inst );
    sectionID ID() const {
        return _sectionID;
    }

    virtual void findSectionStart() = 0;

    void findSectionEnd() {
        _sectionEnd = findNormalString( "ENDSEC", true );
    }

    std::streampos sectionStart() const {
        return _sectionStart;
    }
    std::streampos sectionEnd() const {
        return _sectionEnd;
    }
    void locateAllInstances(); /**< find instances in section, and add lazyInstance's to lazyInstMgr */
    virtual const namedLazyInstance nextInstance() = 0;
    instanceID readInstanceNumber();
};

#endif //SECTIONREADER_H
