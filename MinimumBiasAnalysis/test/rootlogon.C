{
        std::cout << "Loading rootlogon.C" << std::endl;
	gROOT->ProcessLine(".L ~antoniov/tdrstyle.C");
	setTDRStyle();
        gStyle->SetOptStat(1111111);
        gStyle->SetHistLineWidth(2);
        //gStyle->SetHistFillStyle(1);
        gStyle->SetMarkerStyle(23);
        gStyle->SetMarkerSize(0.3);
        gStyle->SetErrorX(0.5);
        gStyle->SetPalette(1);

        gSystem->Load("libFWCoreFWLite");
        AutoLibraryLoader::enable();
        gSystem->Load("libDataFormatsFWLite.so");
        //gSystem->Load("libDataFormatsPatCandidates.so");
        //gSystem->Load("libMinimumBiasAnalysisMinimumBiasAnalysis");
        gSystem->Load("libForwardAnalysisForwardTTreeAnalysis");
        gSystem->Load("libUtilitiesPlottingTools");
        //gSystem->Load("RooUnfold-1.0.3/libRooUnfold.so");
        //gSystem->Load("RooUnfold-1.1.1/libRooUnfold.so");
        //gSystem->Load("libRooFit");
        //gSystem->Load("libRooFitCore");
}
