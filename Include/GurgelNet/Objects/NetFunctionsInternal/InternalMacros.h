#pragma once

#define NF_M_SELECTOR(_1, _2, _3, NAME, ...) NAME

#define NetFunc_0Arg( Loc, Name ) void Name##_ImplNOCALL(); TNetFuncHandle_0< Loc > Name = TNetFuncHandle_0< Loc >( [&](){ Name##_ImplNOCALL(); } )
#define NetFunc_1Arg( Loc, Name, Arg1T ) void Name##_ImplNOCALL( const Arg1T & ); TNetFuncHandle_1< Loc, Arg1T > Name = TNetFuncHandle_1< Loc, Arg1T >( [&]( const Arg1T & a ){ Name##_ImplNOCALL( a ); } )
#define NetFunc_2Arg( Loc, Name, Arg1T, Arg2T ) void Name##_ImplNOCALL( const Arg1T & , const Arg2T & ); TNetFuncHandle_2< Loc, Arg1T, Arg2T > Name = TNetFuncHandle_2< Loc, Arg1T, Arg2T >( [&]( const Arg1T & a, const Arg2T & b ){ Name##_ImplNOCALL( a, b ); } )
#define NetFunc_3Arg( Loc, Name, Arg1T, Arg2T, Arg3T ) void Name##_ImplNOCALL( const Arg1T & , const Arg2T & , const Arg3T & ); TNetFuncHandle_3< Loc, Arg1T, Arg2T, Arg3T > Name = TNetFuncHandle_3< Loc, Arg1T, Arg2T >( [&]( const Arg1T & a, const Arg2T & b, const Arg3T & c ){ Name##_ImplNOCALL( a, b, c ); } )