// Include the top-level Vertica SDK file
#include "Vertica.h"
#include <algorithm>
// Using the Vertica namespace means we don't have to prefix all
// class references with Vertica::
using namespace Vertica;
using namespace std;
/*
 * ScalarFunction implementation for a UDSF that adds
 * two numbers together.
 */
class isnumeric : public ScalarFunction
{
public:
  /*
   * This function does all of the actual processing for the UDF.
   * In this case, it simply reads two integer values and returns
   * their sum.
   *
   * The inputs are retrieved via arg_reader
   * The outputs are returned via arg_writer
   */


  virtual void processBlock(ServerInterface &srvInterface,
                            BlockReader &arg_reader,
                            BlockWriter &res_writer)
  {
    // While we have input to process
    do
      {
        // Read the two integer input parameters by calling the
        // BlockReader.getIntRef class function
        std::string  inStr  = arg_reader.getStringRef(0).str();

        size_t pos;
        while( (pos=inStr.find('.')) != string::npos || (pos=inStr.find(',')) != string::npos   )
                 inStr.erase(pos, 1);


                int sizeOfString = inStr.length();
                int iteration = 0;
                bool isNumeric = true;

                while(iteration < sizeOfString)
                {
                        if(!isdigit(inStr[iteration]))
                        {
                                isNumeric = false;
                                break;
                        }

                        iteration++;

                }



        // Call BlockWriter.setInt to store the output value, which is the
        //  two input values added together
        res_writer.setBool(isNumeric);
        // Finish writing the row, and advance to the next output row
        res_writer.next();
        // Continue looping until there are no more input rows
      }
    while (arg_reader.next());
  }
};

/*
* This class provides metadata about the ScalarFunction class, and
* also instantiates a member of that class when needed.
*/
class isnumericFactory : public ScalarFunctionFactory
{
  // return an instance of Add2Ints to perform the actual addition.
  virtual ScalarFunction *createScalarFunction(ServerInterface &interface)
  {
    // Calls the vt_createFuncObj to create the new Add2Ints class instance.
    return vt_createFuncObj(interface.allocator, isnumeric);
  }
  // This function returns the description of the input and outputs of the
  // Add2Ints class's processBlock function.  It stores this information in
  // two ColumnTypes objects, one for the input parameters, and one for
  // the return value.
  virtual void getPrototype(ServerInterface &interface,
                            ColumnTypes &argTypes,
                            ColumnTypes &returnType)
  {
    // Takes two ints as inputs, so add ints to the argTypes object
    argTypes.addVarchar();

    // returns a single int, so add a single int to the returnType object.
    // Note that ScalarFunctions *always* return a single value.
    returnType.addBool();
  }
};

// Register the factory with HP Vertica
RegisterFactory(isnumericFactory);
