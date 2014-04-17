/* Specific - Minimal C++ spec framework.
 

The zlib/libpng License


Copyright (c) 2008 Mikko Lehtonen

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/


#ifndef SPECIFIC_SPEC_H
#define SPECIFIC_SPEC_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace specific {


    class SpecResult {
    public:
        typedef enum {
            PASSED,
            FAILED,
            ERRORED
        } Type;
      
        Type type;
        
        std::string group;
        std::string description;
        std::string test;
    };


    class SpecFailure {
    public:
        SpecFailure(std::string amsg, const char* afile, int aline)
            : msg(amsg), file(afile), line(aline) { }
        std::string msg;
        const char* file;
        int line;
    };


    class SpecWriter {
    public:
        std::vector<SpecResult> mResults;
        std::vector<SpecFailure> mFailures;
        SpecWriter() {}
        virtual ~SpecWriter() {}
        virtual void startGroup(std::string group, std::string description);
        virtual void addFailedAssertation(std::string msg, const char *file, int line);
        virtual void addSpecResult(SpecResult r);
        virtual void start();
        virtual void stop();
    };


    class ProgressWriter : public SpecWriter {
    public:
        void addSpecResult(SpecResult r);
    };



    class SpecdocWriter : public SpecWriter {
    public:
        void startGroup(std::string group, std::string description);
        void addSpecResult(SpecResult r);
    };



    template<class T> std::string inspect(const T& value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }


    class SpecBase {
    public:
        SpecBase();
        virtual ~SpecBase();

        virtual void specify() = 0;

        void setWriter(SpecWriter* w) { mWriter = w; }

        bool startSpec(const char* name);
        void endSpec();

        void should_test(bool value, const char* message, const char* file, int line);

        template<typename T1, typename T2> void should_equal_template(const T1& a, const T2& b, const char* file, int line) {
            std::stringstream ss;
            ss << "`" << ::specific::inspect(a) << "'" << " == " << "`" << ::specific::inspect(b) << "'";
            should_test( a == b, ss.str().c_str(), file, line);
        }

        template<typename T1, typename T2> void should_not_equal_template(const T1& a, const T2& b, const char* file, int line) {
            std::stringstream ss;
            ss << "`" << ::specific::inspect(a) << "'" << " != " << "`" << ::specific::inspect(b) << "'";
            should_test( a != b, ss.str().c_str(), file, line);
        }



        virtual std::string getGroup() = 0;
        virtual std::string getDescription() = 0;

        bool isSuccessful() { return !mFailed; }
        
        bool done();
        
        void error(std::string msg);

        SpecWriter* mWriter;
        const char* mName;
        bool mFailed;
        bool mLastFailed;
        bool mError;
        int mExecutionPoint;
        int mContinuePoint;
        char *mFile;
        std::string mErrorMessage;
        int mLine;
    };


    class SpecRunner {
    public:
        static SpecRunner& getInstance();
        void add(SpecBase* spec) { mSpecs.push_back( spec ); }
        bool run(SpecWriter& writer, const std::string subset = "");
    private:

        std::vector<SpecBase*> mSpecs;

        SpecRunner();
        ~SpecRunner();
    };

    #define SPEC_UNIQUE_NAME3(x,y) x##y
    #define SPEC_UNIQUE_NAME2(x,y) SPEC_UNIQUE_NAME3(x,y)

    #define SPEC_NAME(x) SPEC_UNIQUE_NAME2(SPEC_##x, SPEC_UNIQUE_NAME2(_startingOnLine, __LINE__) )


    #define describe(group, description)                                    \
    class SPEC_NAME(group) : public specific::SpecBase                         \
    {                                                                       \
    public:                                                                 \
        void specify();                                                     \
        std::string getGroup() { return #group; }                           \
        std::string getDescription() { return description; }                \
    };                                                                      \
    static SPEC_NAME(group) SPEC_UNIQUE_NAME2(SPEC_NAME(group), _instance); \
    void SPEC_NAME(group)::specify()
    

    #define it(description) if(startSpec(description))


    // Matchers
    #define should_be_true(a) should_test(a, #a, __FILE__, __LINE__)
    #define should_be_false(a) should_be_true( !a )

    #ifndef SPECIFIC_NO_OSTREAM
        #define should_equal(a, b) should_equal_template( a,b, __FILE__, __LINE__ )
        #define should_not_equal(a, b) should_not_equal_template( a,b, __FILE__, __LINE__ )
    #else
        #define should_equal(a, b) should_be_true( (a) == (b) )
        #define should_not_equal(a, b) should_be_true( (a) != (b) )
    #endif

    #define should_throw(code, what) \
    do {                             \
        bool _thrown = false;        \
        try {                        \
          code ;                     \
        } catch(what& e) {           \
            _thrown = true;          \
        }                            \
        should_test(_thrown, "should throw exception " #what, __FILE__, __LINE__); \
    } while(0)



}



#endif /* Include guard */

