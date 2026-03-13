set(EXP_DIRS reader writer)

foreach(DIR ${EXP_DIRS})
    add_exp_target(${DIR})
    link_exp_target(${DIR} ivshm pthread m yalantinglibs::yalantinglibs)
endforeach()
