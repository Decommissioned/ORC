#ifndef _SCOPE_GUARD_H
#define _SCOPE_GUARD_H

#define _PP_CAT_(A,B) A ## B
#define PP_CAT_(A,B) _PP_CAT_(A,B)

#define DEFER(exp) auto PP_CAT_(__lambda__,__LINE__) = [&]() { exp; }; __scope__guard__<decltype(PP_CAT_(__lambda__,__LINE__))> PP_CAT_(__SCOPE_GUARD__, __LINE__) (PP_CAT_(__lambda__,__LINE__)) \



template <typename L>
struct __scope__guard__
{
        L func;
        explicit __scope__guard__(L& func) : func(func) {}
        ~__scope__guard__()
        {
                func();
        }
};

#endif // _SCOPE_HUARD_H
