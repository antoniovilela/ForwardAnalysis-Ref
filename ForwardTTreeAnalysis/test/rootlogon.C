{
        std::cout << "Loading rootlogon.C" << std::endl;
	gROOT->ProcessLine(".L tdrstyle.C");
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
        gSystem->Load("libDataFormatsPatCandidates.so");
        gSystem->Load("libForwardAnalysisForwardTTreeAnalysis");
        //gSystem->Load("libUtilitiesPlottingTools");
}
