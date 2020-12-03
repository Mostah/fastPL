#include "gtest/gtest.h"
#include "../src/example.h"
#include <utility>
#include <sstream>
 
TEST( TestNombre, TestNombreVide )
{ 
    Nombre n;
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}