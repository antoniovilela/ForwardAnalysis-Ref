#ifndef UtilitiesPlottingTools_PlottingTools_h
#define UtilitiesPlottingTools_PlottingTools_h

class TFile;
class TDirectory;

#include "TH1F.h"

#include <vector>
#include <string>
#include <map>
#include <exception>

class DefaultNorm{
   public:
      /*DefaultNorm() {}
      double GetNormFactor(const TH1F* hist) {return hist->GetSumOfWeights();}
      double operator()(const TH1F* hist) {return GetNormFactor(hist);}*/

      static double GetNorm(const TH1F* hist) {return hist->GetSumOfWeights();}
   private:
};

class ConstNorm{
   public:
      /*ConstNorm(double norm=1.):normalization_(norm) {}
      double GetNormFactor(const TH1F* hist) {return normalization_;}
      double operator()(const TH1F* hist) {return GetNormFactor(hist);}*/
      
      static double GetNorm(const TH1F* hist) {return normalization;}
   private:
      //double normalization_; 
      static double normalization;
};

class NumberEntriesNorm{
   public:
      /*NumberEntriesNorm() {}
      double operator()(const TH1F* hist) {return hist->GetSumOfWeights()/hist->GetEntries();}*/

      static double GetNorm(const TH1F* hist) {return hist->GetSumOfWeights()/hist->GetEntries();}
   private:
};

/*template <typename T>
class Sum{
   public:
      typedef T value_type;

      Sum(T const& init): sum_(init) {}
      void operator()(T const& x) {sum_ += x;}
      T const value() const {return sum_;}
   private:
      T sum_;
};

template <typename T>
class Mult{
   public:
      typedef T value_type;
 
      Mult(T const& x): val_(x) {}
      T const operator()(T const& x) {return x*val_;}
   private:
      T val_;
};*/

class RootException: public std::exception{
   public:
      RootException(std::string const& msg = ""): message_(msg) {}
      ~RootException() throw() {}

      virtual char const* what() const throw() {
         return message_.c_str();
      }

   private:
      std::string message_;
};
 
//TH1F* getHisto(TFile*, const std::string&); 
TH1F* getHisto(TDirectory const*, std::string const&);
void scaleHisto(TH1F* histo, double scale = 1., int line = 1, int color = 1, int rebin = 1);
TH1F* rebinHisto(TH1F const* histo, std::vector<int> const& groups);
std::map<std::string,std::vector<std::string> > buildVarMap(const std::vector<std::string>& varNames,const std::vector<std::string>& triggerBits);


template<typename KeyType,typename ValueType>
std::map<KeyType,ValueType> makeMap(const std::vector<KeyType>& keys,const std::vector<ValueType>& values){
   std::map<KeyType,ValueType> res;
   typename std::vector<KeyType>::const_iterator key = keys.begin();
   typename std::vector<ValueType>::const_iterator value = values.begin(); 
   typename std::vector<KeyType>::const_iterator keys_end = keys.end();
   typename std::vector<ValueType>::const_iterator values_end = values.end();
   for(; key != keys_end && value != values_end; ++key,++value) res[*key] = *value;

   return res;
}

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm = false);

//template <class NormPolicy>
//void plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, NormPolicy& norm);
#endif
