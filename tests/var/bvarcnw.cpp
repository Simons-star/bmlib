/*################################################################################
  ##
  ##   Copyright (C) 2011-2018 Keith O'Hara
  ##
  ##   This file is part of the BM++ C++ library.
  ##
  ##   BM++ is free software: you can redistribute it and/or modify
  ##   it under the terms of the GNU General Public License as published by
  ##   the Free Software Foundation, either version 2 of the License, or
  ##   (at your option) any later version.
  ##
  ##   BM++ is distributed in the hope that it will be useful,
  ##   but WITHOUT ANY WARRANTY; without even the implied warranty of
  ##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ##   GNU General Public License for more details.
  ##
  ##   You should have received a copy of the GNU General Public License
  ##   along with BM++. If not, see <http://www.gnu.org/licenses/>.
  ##
  ################################################################################*/

/*
 * BVAR-CNW example
 */

#include "bmpp.hpp"

int main()
{
    bm::comptime begin_time = bm::tic();

    int n = 1000;
    int sim_burn = 1000;

    bool cons_term = true;
    int p = 2;
    int M = 2;

    arma::mat beta(5,2);
    beta <<  7.0  <<  3.0  << arma::endr
         <<  0.50 <<  0.20 << arma::endr
         <<  0.28 <<  0.70 << arma::endr
         << -0.39 << -0.10 << arma::endr
         <<  0.10 <<  0.05 << arma::endr;

    //

    arma::mat Y = bm::var_sim(beta,cons_term,n,sim_burn);

    //
    
    bm::bvarcnw bvar_model;

    bvar_model.build(Y,cons_term,p);

    //

    arma::vec coef_prior = arma::zeros(M,1) + 0.5;

    double HP_1 = 0.5;
    double HP_2 = 1.0;
    int gamma = M+1;

    bvar_model.prior(coef_prior,HP_1,HP_2,gamma);

    //

    int n_draws  = 10000;

    bvar_model.gibbs(n_draws);

    arma::cout << "alpha mean:\n" << bvar_model.alpha_pt_mean << arma::endl;
    arma::cout << "alpha var:\n" << bvar_model.alpha_pt_var << arma::endl;
    arma::cout << "Sigma mean:\n" << bvar_model.Sigma_pt_mean << arma::endl;

    //

    int n_irf_periods = 20;

    arma::cube irfs = bvar_model.IRF(n_irf_periods);

    arma::cout << "IRFs:\n" << irfs.slice(0) << arma::endl;

    //

    int forecast_horizon = 10;

    arma::cube forecasts = bvar_model.forecast(forecast_horizon,false);

    arma::cout << "Forecasts:\n" << forecasts.slice(0) << arma::endl;

    //

    bm::tictoc(begin_time);

    return 0;
}
