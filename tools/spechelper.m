#!/usr/bin/env octave

1;

function spec_formatter (val)

    if( isscalar(val) == 1 ) 
        printf("        should_be_close_to(x, %f, epsilon );", val);
        return;
    endif


    if( size(val) == [1,4] ) 
        printf("        should_be_equal_simd4f(x, simd4f_create(%f, %f, %f, %f), epsilon );", val(1), val(2), val(3), val(4));
        return;
    endif

endfunction

