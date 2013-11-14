#ifndef ATTRIBUTES_GEN_ExportsGenerator_H
#define ATTRIBUTES_GEN_ExportsGenerator_H

namespace attributes{

    // Abstract class which manages writing of code for compileAttributes
    class ExportsGenerator {
    protected:
        ExportsGenerator(const std::string& targetFile, 
                         const std::string& package,
                         const std::string& commentPrefix);
 
    private:
        // prohibit copying
        ExportsGenerator(const ExportsGenerator&);
        ExportsGenerator& operator=(const ExportsGenerator&); 
        
    public:
        virtual ~ExportsGenerator() {}
        
        // Name of target file and package
        const std::string& targetFile() const { return targetFile_; }
        const std::string& package() const { return package_; }
        
        // Abstract interface for code generation
        virtual void writeBegin() = 0;
        void writeFunctions(const SourceFileAttributes& attributes,
                            bool verbose); // see doWriteFunctions below
        virtual void writeEnd() = 0;
        
        virtual bool commit(const std::vector<std::string>& includes) = 0;
        
        // Remove the generated file entirely
        bool remove(); 
        
        // Allow generator to appear as a std::ostream&
        operator std::ostream&() {
            return codeStream_;
        }
        
    protected: 
    
        // Allow access to the output stream 
        std::ostream& ostr() {
            return codeStream_;
        }
        
        bool hasCppInterface() const {
            return hasCppInterface_;
        }
        
        // Shared knowledge about function namees
        std::string exportValidationFunction() {
            return "RcppExport_validate";
        } 
        std::string exportValidationFunctionRegisteredName() {
            return package() + "_" + exportValidationFunction();
        } 
        std::string registerCCallableExportedName() {
            return package() + "_RcppExport_registerCCallable";
        }

        // Commit the stream -- is a no-op if the existing code is identical
        // to the generated code. Returns true if data was written and false
        // if it wasn't (throws exception on io error)
        bool commit(const std::string& preamble = std::string()); 
        
    private:
    
        // Private virtual for doWriteFunctions so the base class 
        // can always intercept writeFunctions
        virtual void doWriteFunctions(const SourceFileAttributes& attributes,
                                      bool verbose) = 0;
    
        // Check whether it's safe to overwrite this file (i.e. whether we 
        // generated the file in the first place)
        bool isSafeToOverwrite() const {
            return existingCode_.empty() || 
                   (existingCode_.find(generatorToken()) != std::string::npos);
        }
        
        // UUID that we write into a comment within the file (so that we can 
        // strongly identify that a file was generated by us before overwriting it)
        std::string generatorToken() const {
            return "10BE3573-1514-4C36-9D1C-5A225CD40393";
        }
    
    private:
        std::string targetFile_;
        std::string package_;
        std::string commentPrefix_;
        std::string existingCode_;
        std::ostringstream codeStream_;
        bool hasCppInterface_;
    };
    
}

#endif
