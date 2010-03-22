#!/usr/bin/env octave

1;

function spec_formatter (val,type)

    if( isscalar(val) == 1 ) 
        printf("        should_be_close_to(x, %f, epsilon );", val);
        return;
    endif

    if( size(val) == [1,3] ) 
        printf("        should_be_equal_%s(x, simd4f_create(%f, %f, %f, 0), epsilon );",type, val(1), val(2), val(3));
        return;
    endif

    if( size(val) == [1,4] ) 
        printf("        should_be_equal_%s(x, simd4f_create(%f, %f, %f, %f), epsilon );",type, val(1), val(2), val(3), val(4));
        return;
    endif

endfunction

