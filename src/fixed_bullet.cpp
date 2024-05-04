#include <Rcpp.h>
using namespace Rcpp;

#include <testthat.h>
#include "amortr.h"

// amortr::fixed_bullet(10000, 0.01, as.Date("2020-01-01"), NULL, 20, 12, NULL)
// amortr::fixed_bullet(10000, 0.01, as.Date("2020-01-01"), NULL, 20, 12, c("hola"="yes"))
// amortr::fixed_bullet(10000, 0.01, as.Date("2020-01-01"), as.Date("2022-01-01"), NULL, 12, NULL)
// amortr::fixed_bullet(10000, 0.01, as.Date("2020-01-01"), as.Date("2022-01-01"), NULL, 12,  c("hola"="yes"))

//' Fixed-rate bullet loan amortization schedule calculation function.
 //'
 //' @param amount initial principal amount
 //' @param interest_rate yearly interest rate
 //' @param start_date date of first loan payment
 //' @param end_date date in which the last loan payment is expected. This argument overrides n_payments.
 //' @param n_payments number of payments expected.
 //' @param frequency number of months in a year in which payments are expected
 //' @param optional_parameters named vector containing constant columns to be added to the return dataframe
 //' @param truncate_month_day if true, all the days in the return dataframe dates column will be set to zero 
 //' @return dataframe containing the amortization schedule for the loan described in the arguments
 // [[Rcpp::export]]
 Rcpp::DataFrame fixed_bullet_(
     int64_t amount, // principal amount
     float interest_rate, // interest rate of the loan
     Date start_date,  // date in which the loan is taken out, i.e. when the borrower receives the principal amount.
     Nullable<Date> end_date = R_NilValue, // final date of the loan
     Nullable<NumericVector> n_payments = R_NilValue, // number of periods of the loan, i.e. number of payments from borrower to lender
     int frequency = 12, // number of payments done in every year
     Nullable<List> optional_parameters = R_NilValue, // optional columns to be added to the dataframe
     bool truncate_month_day= true // trunk month payment_date to the first day of the month
 ) {
   // calculate the range of payment_date
   // Rcpp::DataFrame amortization_schedule_df = Rcpp::DataFrame::create(Rcpp::Named("payment_date"));
   int total_periods;
   
   if(frequency > 0) {
     // frequency must be a multiple of 2 and 3
     if(!(frequency == 1 || frequency == 2 || frequency == 3 || frequency == 4 || frequency == 6 || frequency == 12)) {
       stop("Frequency must be one of the following: 1, 2, 3, 4, 6 or 12.");
     }
     
   } else {
     stop("frequency cannot be 0.");
   }
   
   DateVector date_range { generate_date_range(
       start_date,  // date in which the loan is taken out, i.e. when the borrower receives the principal amount.
       end_date, // final date of the loan
       n_payments, // number of periods of the loan, i.e. number of payments from borrower to lender
       frequency, // number of payments done in every year
       truncate_month_day // trunk month payment_date to the first day of the month
   )};
   
   total_periods = date_range.length();
   
   // Rcout << "total_periods: " << total_periods << std::endl;
   
   // initialize vectors that will be bundled into the dataframe to return
   DateVector payment_date(total_periods);
   NumericVector balance(total_periods);
   balance[0] = amount;
   NumericVector interest_payment(total_periods);
   NumericVector principal_payment(total_periods);
   NumericVector total_payment(total_periods);
   
   // calculate the schedule of payments
   
   balance[0] = amount;

   for (int index=1; index < (total_periods - 1); index++) {
     interest_payment[index] = std::round(interest_rate*amount / 0.01) * 0.01;
     principal_payment[index] = 0;
     balance[index] = amount;
     total_payment[index] = interest_payment[index];
   }
   
   // this is a bit of a dirty trick to avoid dealing with the intricacies of floating comma rounding
   // without this, the last principal payment would -at most, but in most cases- slightly deviate from the final balance payment
   interest_payment[total_periods - 1] = std::round(interest_rate*amount / 0.01) * 0.01;
   principal_payment[total_periods - 1] = amount;
   balance[total_periods-1] = amount;
   total_payment[total_periods-1] = interest_payment[total_periods-1] + principal_payment[total_periods - 1];
   
   // build dataframe to return
   DataFrame return_dataframe = DataFrame::create(
     Named("payment_date") = date_range,
     Named("total_payment") = total_payment,
     Named("interest_payment") = interest_payment,
     Named("principal_payment") = principal_payment,
     Named("balance") = balance,
     Named("interest_rate") = interest_rate
   );
   
   // add optional values to schedule as columns with single value
   if(optional_parameters.isNotNull()) {
     Rcpp::List optional_parameters_(optional_parameters);
     
     Rcpp::CharacterVector optional_parameters_names = optional_parameters_.names();
     
     for(int i=0; i < optional_parameters_.size(); i++) {
       return_dataframe[std::string(optional_parameters_names[i])] = optional_parameters_[i];
     }
     
     return Rcpp::DataFrame(return_dataframe); // converting back to dataframe because adding columns causes fallback into list
   } else {
     return return_dataframe;
   }
   
 }
 
 
 