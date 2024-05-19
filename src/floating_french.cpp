#include <Rcpp.h>
using namespace Rcpp;

#include <testthat.h>
#include "amortr.h"

// amortr::fixed_french(10000, 0.01, as.Date("2020-01-01"), NULL, 20, 12, NULL)
// amortr::fixed_french(10000, 0.01, as.Date("2020-01-01"), NULL, 20, 12, c("hola"="yes"))
// amortr::fixed_french(10000, 0.01, as.Date("2020-01-01"), as.Date("2022-01-01"), NULL, 12, NULL)
// amortr::fixed_french(10000, 0.01, as.Date("2020-01-01"), as.Date("2022-01-01"), NULL, 12,  c("hola"="yes"))

//' Floating-rate french loan amortization schedule calculation function.
 //'
 //' @param amount initial principal amount.
 //' @param interest_rate yearly interest rate.
 //' @param start_date date of first loan payment.
 //' @param end_date date in which the last loan payment is expected. This argument overrides n_payments.
 //' @param n_payments number of payments expected.
 //' @param frequency number of months in a year in which payments are expected
 //' @param optional_parameters named vector containing constant columns to be added to the return dataframe
 //' @param truncate_month_day if true, all the days in the return dataframe dates column will be set to zero 
 //' @param interest_rate_schedule dataframe containing dates in the first column, and their corresponding interest rates in the second column. Applicable rates beyond the last row in the dataframe will be exstended to the future until end_date or the date that results from adding .n_payments to .start_date
 //' @param recalculation_frequency number of months until a new amortization schedule is calculated with. 
 //' @examples
 //' @return a dataframe containing the amortization schedule for the loan described in the arguments
 // [[Rcpp::export]]
 Rcpp::DataFrame float_french_(
     int64_t amount, // principal amount
     float interest_rate, // interest rate of the loan
     Date start_date,  // date in which the loan is taken out, i.e. when the borrower receives the principal amount.
     Nullable<Date> end_date = R_NilValue, // final date of the loan
     Nullable<NumericVector> n_payments = R_NilValue, // number of periods of the loan, i.e. number of payments from borrower to lender
     int frequency = 12, // number of payments done in every year
     Nullable<List> optional_parameters = R_NilValue, // optional columns to be added to the dataframe
     bool truncate_month_day= true, // trunk month payment_date to the first day of the month
     Nullable<Rcpp::DataFrame> interest_rate_schedule_ = R_NilValue,
     int recalculation_frequency = 12 
 ) {
   
   // check that all required arguments are not NULL
   if(frequency > 0) {
     // frequency must be a multiple of 2 and/or 3
     if(!(frequency == 1 || frequency == 2 || frequency == 3 || frequency == 4 || frequency == 6 || frequency == 12)) {
       stop("Frequency must be one of the following: 1, 2, 3, 4, 6 or 12.");
     }
   } else {
     stop("frequency cannot be 0.");
   }
   
   // date_range: obtain the full date range for the loan
   int total_periods;
   int n_recalculations;
   
   DateVector date_range { 
     generate_date_range(
       start_date,  // date in which the loan is taken out, i.e. when the borrower receives the principal amount.
       end_date, // final date of the loan
       n_payments, // number of periods of the loan, i.e. number of payments from borrower to lender
       frequency, // number of payments done in every year
       truncate_month_day // trunk month payment_date to the first day of the month
     )};
   
   total_periods = date_range.length();
   n_recalculations = total_periods / recalculation_frequency;
   
   // interest_rate_schedule: process the DataFrame containing dates and interest rates
   if(interest_rate_schedule_.isNull()) {
     stop("An interest rate schedule must be provided as a DataFrame with at least two columns.");
   } 
   
   Rcpp::DataFrame interest_rate_schedule_raw(interest_rate_schedule_); 
   NumericVector interest_rate_schedule(n_recalculations);
   
   if(interest_rate_schedule_raw.ncol() < 2 || interest_rate_schedule_raw.nrow() == 0) {
     stop("An interest rate schedule must be provided as a DataFrame with at least two columns.");
   }
    
   int recalculations_with_extrapolated_rate; 
   bool toggle_first_day_in_month;
   
   // only the first day of the month will be taken, if there are more than one day in a given month 
   DateVector dates_col = interest_rate_schedule_raw[0]; 
   NumericVector rates_col = interest_rate_schedule_raw[1];
   int interest_rate_schedule_counter = 0;
   
   /*
   for(int i=0; i < interest_rate_schedule_raw.nrows(); i++) {
     if(dates_col[i].getDay() > 1 && !toggle_first_day_in_month) {
       // this row contains a date which is either the first day of the month, or at least the earliest day in the month to be iterated through
       interest_rate_schedule[i++] = rates_col[i];
     }
   }
   */
   
   // substracting from n_recalculations the number of recalculations in which the interest rate has been extrapolated from the latest available
   // these recalculations periodo do not need to be computed, as their interest rate will stay constant
   n_recalculations = n_recalculations - recalculations_with_extrapolated_rate;
   
   // initialize vectors that will be bundled into the returned dataframe 
   DateVector payment_date(total_periods);
   NumericVector balance(total_periods);
   balance[0] = amount;
   NumericVector interest_payment(total_periods);
   NumericVector principal_payment(total_periods);
   double total_payment;
   
   
   // one iteration for every recalculation period
   for(int recalculation_period=0; recalculation_period < n_recalculations; recalculation_period++) {
     interest_rate = interest_rate_schedule[recalculation_period]; 
     
     // calculate the schedule of payments
     total_payment =  amount/
       ( 1-pow((1+interest_rate),
               -total_periods))*interest_rate;
     
     total_payment = std::round(total_payment/0.01)*0.01;
     
     // each of the recalculation periods starts recalculation_frequency months after the previous one
     for (int index=recalculation_period*recalculation_frequency; index < (total_periods - 1); index++) {
       interest_payment[index] = std::round(interest_rate*balance[index] / 0.01) * 0.01;
       principal_payment[index] = std::round((total_payment - interest_payment[index])/0.01) * 0.01;
       balance[index+1] = balance[index] - principal_payment[index];
     }
     
   }
   
   
   // this is a bit of a dirty trick to avoid dealing with the intricacies of floating comma rounding
   // without this, the last principal payment would -at most, but in most cases- slightly deviate from the final balance payment
   interest_payment[total_periods - 1 ] = std::round(interest_rate*balance[total_periods - 1] / 0.01) * 0.01;
   principal_payment[total_periods - 1] = total_payment - interest_payment[total_periods - 1];
   balance[total_periods-1] = principal_payment[total_periods - 1];
   
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
 
 
 