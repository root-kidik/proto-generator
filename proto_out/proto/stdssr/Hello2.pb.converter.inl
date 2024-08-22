#pragma once

#include <stdssr/Hello2.hpp>
#include <stdssr/Hello2.pb.h> 


#include <stdssr/sub/Hello.hpp>
#include <stdssr/sub/Hello.pb.converter.inl> 


namespace proto
{


inline stdssr::QWE convert(stdssr_QWE&& dto)
{
    stdssr::QWE object; 
    
        
    object.w = convert(std::move(*dto.mutable_w()));
        
    
}

inline stdssr_QWE convert(stdssr::QWE&& object)
{
    stdssr_QWE dto;
    
        
    *dto.mutable_w() = convert(std::move(object.w));
        
    
}


}
