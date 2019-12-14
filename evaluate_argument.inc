
struc _evaluate_argument? argument?*
    local ret_1, ret_2
    redefine .
    ret_1 _rpn_builder, argument ; don't care about polynomial
    ret_2 _rpn_evaluator ret_1
    irpv computed, ret_2
        redefine . computed
    end irpv
end struc
