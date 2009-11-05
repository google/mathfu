#include "spec_helper.h"

using vectorial::vec4f;

const float epsilon = 0.00001f;

describe(vec4f, "arithmetics") {
    
    it("should add") {
        
        vec4f a(1,2,3,4);
        vec4f b(2,3,4,5);
        
        vec4f c = a + b;
        
        should_be_equal_vec4f( c, vec4f(3,5,7,9), epsilon );
        
    }
    
}
