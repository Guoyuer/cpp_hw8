#ifndef CPP_HW8_8_2_H
#define CPP_HW8_8_2_H

#include<future>

namespace MPCS51044 {
    /* template declaration */
    template<typename F>
    struct fn_concept;

    /* template specialization */
    template<typename Ret, typename... Args>
    struct fn_concept<Ret(Args...)> {
        virtual Ret call(Args &&... args) = 0;

        virtual ~fn_concept() = default;
    };

    /* template declaration */
    template<typename F, typename T>
    struct fn_model;


    /* template specialization */
    template<typename Res, typename... Args, typename T>
    struct fn_model<Res(Args...), T> : fn_concept<Res(Args...)> {
        T functor;

        fn_model(T &&functor) : functor(functor) {};

        Res call(Args &&... args) { return std::invoke(functor, std::forward<Args>(args)...); }
    };

    template<typename F>
    class function;

    template<typename Res, typename... Args>
    class function<Res(Args...)> {
        std::unique_ptr<fn_concept<Res(Args...)>> c;
    public:
        template<typename F>
        function(F &&f) {
            c = std::make_unique<fn_model<Res(Args...), F>>(f);
        }

        template<typename F>
        function &operator=(F &&f) {
            c = std::make_unique<fn_model<Res(Args...), F>>(f);
        }

        Res operator()(Args &&... args) {
            return c->call(std::forward<Args>(args)...);
        }
    };
}


#endif //CPP_HW8_8_2_H
