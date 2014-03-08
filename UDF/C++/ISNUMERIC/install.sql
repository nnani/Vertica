-- This script install the function and its library into Vertica Database

-- Installing Library
create or replace library isnumericFunctions as '/home/dbadmin/navin//UDF/ISNUMERIC/build/isnumeric.so';


-- Installing function into database
create or replace function isnumeric as language 'C++' name 'isnumericFactory' library isnumericFunctions;