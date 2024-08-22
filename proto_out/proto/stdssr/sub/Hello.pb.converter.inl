#pragma once

#include <stdssr/sub/Hello.hpp>
#include <stdssr/sub/Hello.pb.h> 


#include <stdssr/sub/Hello0.hpp>
#include <stdssr/sub/Hello0.pb.converter.inl> 


namespace proto
{


inline stdssr::Some convert(stdssr_Some&& dto)
{
    stdssr::Some object; 
    
        
    object.q = std::move(dto.get_q());
        
    
        
    object.b = std::move(dto.get_b());
        
    
        
    object.ty = convert(std::move(*dto.mutable_ty()));
        
    
}

inline stdssr_Some convert(stdssr::Some&& object)
{
    stdssr_Some dto;
    
        
    dto.set_q(object.q);
        
    
        
    dto.set_b(object.b);
        
    
        
    *dto.mutable_ty() = convert(std::move(object.ty));
        
    
}

inline stdssr2::Q convert(stdssr2_Q&& dto)
{
    stdssr2::Q object; 
    
        
    object.wteryrtrut = convert(std::move(*dto.mutable_wteryrtrut()));
        
    
        
    object.h = std::move(dto.get_h());
        
    
        
    object.w = std::move(dto.get_w());
        
    
        
    object.w3 = std::move(dto.get_w3());
        
    
        
    object.w2 = std::move(dto.get_w2());
        
    
        
    object.tytu = std::move(dto.get_tytu());
        
    
}

inline stdssr2_Q convert(stdssr2::Q&& object)
{
    stdssr2_Q dto;
    
        
    *dto.mutable_wteryrtrut() = convert(std::move(object.wteryrtrut));
        
    
        
    dto.set_h(object.h);
        
    
        
    dto.set_w(object.w);
        
    
        
    dto.set_w3(object.w3);
        
    
        
    dto.set_w2(object.w2);
        
    
        
    dto.set_tytu(object.tytu);
        
    
}


}
