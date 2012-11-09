import ROOT
#from ForwardAnalysis.AnalysisSequences.pfThresholds_cfi import pfThresholds
from ForwardAnalysis.ExclusiveDijetsAnalysis.pfThresholds_cfi import pfThresholds
"""
def getThreshold(pset,region,type,var):
    regionPSet = getattr(pset,region)
    typePSet = getattr(regionPSet,type)
    value = getattr(typePSet,var).value()
    return value
"""

#etaBinsHCALBoundaries = (-5.205, -4.903, -4.730,
#                         -4.552, -4.377, -4.204, -4.027, -3.853, -3.677, -3.503, -3.327, -3.152,
#                         -3.000, -2.868, -2.650, -2.500,
#                         -2.322, -2.172, -2.043, -1.930, -1.830, -1.740, -1.653, -1.566, -1.479,
#                         -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.870, -0.783,
#                         -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087,
#                          0.000, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696,
#                          0.783, 0.870, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392,
#                          1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322,
#                          2.500, 2.650, 2.868, 3.000,
#                          3.152, 3.327, 3.503, 3.677, 3.853, 4.027, 4.204, 4.377, 4.552,
#                          4.730, 4.903, 5.205) #41 + 41 bins
etaBinsHCALBoundaries = (-5.205, -4.903, -4.730,
                         -4.552, -4.377, -4.204, -4.027, -3.853, -3.677, -3.503, -3.327, -3.152,
                         -2.976, -2.866, -2.650, -2.500,
                         -2.322, -2.172, -2.043, -1.930, -1.830, -1.740, -1.653, -1.566, -1.479,
                         -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.870, -0.783,
                         -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087,
                          0.000, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696,
                          0.783, 0.870, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392,
                          1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322,
                          2.500, 2.650, 2.866, 2.976,
                          3.152, 3.327, 3.503, 3.677, 3.853, 4.027, 4.204, 4.377, 4.552,
                          4.730, 4.903, 5.205) #41 + 41 bins

etaBinsHFRegions = etaBinsHCALBoundaries[-14:]
print etaBinsHFRegions

def fitHisto(histo):
    nSigmas = 4
    fitOption = "R"
    iBinMax = histo.GetMaximumBin()
    iBinLow = -1
    for ibin in range(1,iBinMax + 1):
	if histo.GetBinContent(ibin) >= 0.15*histo.GetMaximum():
	    iBinLow = ibin
	    break
    rangeFitLow = histo.GetBinCenter(iBinLow)
    print "Histogram maximum",histo.GetBinCenter(iBinMax),histo.GetMaximum()
    print "Fit low range",rangeFitLow

    minFit = histo.GetBinCenter(iBinMax) - histo.GetRMS()
    if minFit < rangeFitLow: minFit = rangeFitLow
    maxFit = histo.GetBinCenter(iBinMax) + histo.GetRMS()

    funcName = histo.GetName() + "_gaus"
    fitFunc = ROOT.TF1(funcName,"gaus",minFit,maxFit)
    histo.Fit(fitFunc,fitOption)

    minFit = fitFunc.GetParameter(1) - nSigmas*fitFunc.GetParameter(2)
    if minFit < rangeFitLow: minFit = rangeFitLow 
    maxFit = fitFunc.GetParameter(1) + nSigmas*fitFunc.GetParameter(2)
    fitFunc.SetRange(minFit,maxFit)
    histo.Fit(fitFunc,fitOption)

    return (fitFunc.GetParameter(1),fitFunc.GetParameter(2))

def fitHistoDoubleGaus(histo):
    fitOption = "R"
    iBinMax = histo.GetMaximumBin()
    iBinLow = -1
    for ibin in range(1,iBinMax + 1):
	if histo.GetBinContent(ibin) >= 0.15*histo.GetMaximum():
	    iBinLow = ibin
	    break
    rangeFitLow = histo.GetBinCenter(iBinLow)
    maximumHisto = histo.GetMaximum()
    print "Histogram maximum",histo.GetBinCenter(iBinMax),maximumHisto
    print "Fit low range",rangeFitLow

    minFit = histo.GetBinCenter(iBinMax) - histo.GetRMS()
    if minFit < rangeFitLow: minFit = rangeFitLow
    maxFit = histo.GetBinCenter(iBinMax) + histo.GetRMS()

    funcName = histo.GetName() + "_gaus"
    fitFunc = ROOT.TF1(funcName,"gaus",minFit,maxFit)
    histo.Fit(fitFunc,fitOption)

    mean = fitFunc.GetParameter(1)
    sigma = fitFunc.GetParameter(2)
    if mean < rangeFitLow:
        mean = histo.GetBinCenter(iBinMax)

    minFit = mean - 3*sigma
    if minFit < rangeFitLow: minFit = rangeFitLow 
    maxFit = mean + 10*sigma

    funcName = histo.GetName() + "_doubleGaus"
    fitFuncDoubleGaus = ROOT.TF1(funcName,"gaus(0) + gaus(3)",minFit,maxFit)
    fitFuncDoubleGaus.SetParameters(fitFunc.GetParameter(0),mean,sigma,
                                    0.05*fitFunc.GetParameter(0),mean,10*sigma)
 
    fixMean = True
    if fixMean:
        fitFuncDoubleGaus.FixParameter(1,mean)
        fitFuncDoubleGaus.FixParameter(4,mean)

    histo.Fit(fitFuncDoubleGaus,fitOption)

    meanFinal = fitFuncDoubleGaus.GetParameter(4)
    sigmaFinal = fitFuncDoubleGaus.GetParameter(5)
    return (meanFinal,sigmaFinal)

def fitHistoRooFit(histo, plot=False):

    iBinMax = histo.GetMaximumBin()
    iBinLow = -1
    for ibin in range(1,iBinMax + 1):
	if histo.GetBinContent(ibin) >= 0.15*histo.GetMaximum():
	    iBinLow = ibin
	    break

    minHisto = histo.GetBinCenter(1)
    maxHisto = histo.GetBinCenter( histo.GetNbinsX() )
    rangeFitLow = histo.GetBinCenter(iBinLow)
    rangeFitHigh = histo.GetBinCenter( histo.GetNbinsX() )  
    print "Histogram range, maximum",minHisto,maxHisto,histo.GetBinCenter(iBinMax),histo.GetMaximum()
    print "Fit range",rangeFitLow,rangeFitHigh
    
    x = ROOT.RooRealVar(histo.GetName() + "_var","energy",minHisto,maxHisto)
    sigma1 = ROOT.RooRealVar("sigma1","sigma1",0.,0.7)
    sigma2 = ROOT.RooRealVar("sigma2","sigma2",0.,0.7)
    mean1 = ROOT.RooRealVar("mean1","mean1",histo.GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh)
    mean2 = ROOT.RooRealVar("mean2","mean2",histo.GetBinCenter(iBinMax),rangeFitLow,rangeFitHigh)
    
    frac = ROOT.RooRealVar("frac","frac",0.,1.)
   
    gaus1 = ROOT.RooGaussian(histo.GetName() + "_gaus1","Gaussian distribution",x,mean1,sigma1)
    gaus2 = ROOT.RooGaussian(histo.GetName() + "_gaus2","Gaussian distribution",x,mean2,sigma2)
    model = ROOT.RooAddPdf(histo.GetName() + "_model","Model",ROOT.RooArgList(gaus1,gaus2),ROOT.RooArgList(frac))

    hdata = ROOT.RooDataHist(histo.GetName() + "_hdata","Binned data",ROOT.RooArgList(x),histo)
    #model.fitTo( hdata,ROOT.RooFit.Minos(0) )
    #model.fitTo(hdata,ROOT.RooFit.Minos(0),ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
    #model.fitTo(hdata,ROOT.RooFit.Range(rangeFitLow,rangeFitHigh))
    model.fitTo(hdata)

    objects = []
    if plot:
        frame = x.frame()
	hdata.plotOn(frame)
        model.plotOn(frame)
        model.plotOn(frame,ROOT.RooFit.Components( gaus2.GetName() ),
                           ROOT.RooFit.LineColor(2),ROOT.RooFit.LineStyle(2))
        
	#gaus1.plotOn(frame,ROOT.RooFit.LineStyle(3),ROOT.RooFit.LineColor(2))
	#gaus2.plotOn(frame,ROOT.RooFit.LineStyle(2),ROOT.RooFit.LineColor(3))
        
	canvas = ROOT.TCanvas("c_" + histo.GetName() + "_fit")
	canvas.SetGridy()
	#canvas.SetLogy()
	canvas.cd()
        frame.Draw()

        objects.append(x)
        objects.append(frame)
        objects.append(hdata)
        objects.append(model)
        objects.append(gaus1)
        objects.append(gaus2)
        objects.append(canvas) 

    return (mean1,mean2,sigma1,sigma2,frac,objects)

def plot(fileName, selection="pFlowAnalysisNoCollNoVtx", runFit=True, rebin=1, outputFileName=""):

    AddDirectoryStatus_ = ROOT.TH1.AddDirectoryStatus() 
    ROOT.TH1.AddDirectory(False)
    
    file = ROOT.TFile(fileName,'READ')

    h_NEvents = file.Get( "%s/%s" % (selection,"NEvents") )
    nEvents = h_NEvents.GetBinContent(1)

    particleTypes = ("all","gamma","hadronNeutral","hadronHF","emHF")
    #particleTypes = ("all","gamma","hadronNeutral","hadronHF","emHF","hadronHFEcalEnergy","hadronHFNoEcalEnergy")
    histoNames = {}
    histoNames["all"] = "energyVsEtaAllTypes" 
    histoNames["gamma"] = "energyVsEtaGamma"
    histoNames["hadronNeutral"] = "energyVsEtaNeutralHadron"
    histoNames["hadronHF"] = "energyVsEtaHadronHF"
    #histoNames["hadronHF"] = "energyVsEtaHadronHFEcalEnergy"
    histoNames["emHF"] = "energyVsEtaEGammaHF"
    #histoNames["hadronHFEcalEnergy"] = "energyVsEtaHadronHFEcalEnergy"
    #histoNames["hadronHFNoEcalEnergy"] = "energyVsEtaHadronHFNoEcalEnergy"
    histoNamesSumEnergyVsEta = {}
    histoNamesSumEnergyVsEta["all"] = "sumEnergyVsEtaAllTypes" 
    histoNamesSumEnergyVsEta["gamma"] = "sumEnergyVsEtaGamma"
    histoNamesSumEnergyVsEta["hadronNeutral"] = "sumEnergyVsEtaNeutralHadron"
    histoNamesSumEnergyVsEta["hadronHF"] = "sumEnergyVsEtaHadronHF"
    histoNamesSumEnergyVsEta["emHF"] = "sumEnergyVsEtaEGammaHF"
    regions = {}
    #regions["Barrel"] = (0.,1.4)
    #regions["Endcap"] = (1.4,2.6) 
    #regions["Transition"] = (2.6,3.2)
    #regions["Forward"] = (3.2,5.205)
    regions["Barrel"] = (0.,1.392)
    regions["Endcap"] = (1.392,2.650) 
    regions["Transition"] = (2.650,3.152)
    #regions["Forward"] = (3.152,5.205)
    regions["Forward"] = (3.152,4.730)
    for idx in range( len(etaBinsHFRegions) - 1 ):
        regions["HFRing%d" % (idx + 1)] = (etaBinsHFRegions[idx],etaBinsHFRegions[idx + 1]) 

    """
    thresholds = {}
    thresholds["all"] = {}
    thresholds["gamma"] = {"Barrel":0.9,
                           "Endcap":2.5,
                           "Transition":2.5}
    thresholds["hadronNeutral"] = {"Barrel":1.4,
                                   "Endcap":2.7,
                                   "Transition":3.8}
    thresholds["hadronHF"] = {"Transition":4.0,
                              "Forward":4.0}
    thresholds["emHF"] = {"Transition":3.5,
                          "Forward":3.5}
    """
    thresholds = {}
    for type in particleTypes:
        thresholds[type] = {} 
        if type == 'all': continue
        for region in regions:
            if not hasattr(pfThresholds,region): continue
	    regionPSet = getattr(pfThresholds,region)
            if hasattr(regionPSet,type):
		typePSet = getattr(regionPSet,type)
                if hasattr(typePSet,'energy'):
		    value = getattr(typePSet,'energy').value()
		    thresholds[type][region] = value

    for idx in range( len(etaBinsHFRegions) - 1 ):
        thresholds["hadronHF"]["HFRing%d" % (idx + 1)] = 0. 
        thresholds["emHF"]["HFRing%d" % (idx + 1)] = 0. 

    print thresholds

    canvases = []
    canvasesProfX = []
    canvasesProjX = []
    histos = []
    profilesX = []
    projectionsX = []
    objects = []
    thresholdsPlus = {}
    thresholdsMinus = {}
    for type in particleTypes:
        #if type == 'all': continue
   
        histo = file.Get("%s/%s" % (selection,histoNames[type]))
        canvas = ROOT.TCanvas("c_" + histo.GetName())
        canvas.SetGridy()
        canvas.cd() 
        
        histo.SetMarkerSize(0.1)
        #histo.SetMarkerColor(4)
        histo.GetXaxis().SetTitle("#eta")
        histo.GetYaxis().SetTitle("Energy (GeV)")
        histo.SetStats(0)
        histo.Scale( 1./nEvents )
        histo.Draw("COLZ")

        lines = []
        canvasesProj = []         
        projections = []
        linesProj = []
        fitObjects = []
        thresholdsPlus[type] = {}
        thresholdsMinus[type] = {}
        for region in thresholds[type]:
            xrange = regions[region]
            ythreshold = 0.
            if thresholds[type].has_key(region): ythreshold = thresholds[type][region]
            print "Threshold for %s, %s region, eta=[%.1f,%.1f): %.1f" % (type,region,xrange[0],xrange[1],ythreshold)

            canvas.cd()
            lines.append(ROOT.TLine(xrange[0],ythreshold,xrange[1],ythreshold))
            lines[-1].SetLineColor(2)
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(6)
            lines[-1].Draw("SAME")
            lines.append(ROOT.TLine(-xrange[1],ythreshold,-xrange[0],ythreshold))
            lines[-1].SetLineColor(2) 
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(6)
            lines[-1].Draw("SAME")

            # Projections
            canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zplus")
            canvasesProj.append(canvasProj)
            canvasesProj[-1].SetGridy()
            canvasesProj[-1].SetLogy()
            canvasesProj[-1].cd() 
            
            iBinLow = histo.GetXaxis().FindBin(xrange[0])
            iBinUpp = histo.GetXaxis().FindBin(xrange[1]) - 1
            print "Bin range for %s, %s region, eta=[%.1f,%.1f): [%d,%d]" % (type,region,xrange[0],xrange[1],iBinLow,iBinUpp)
            projectionY = histo.ProjectionY(histo.GetName() + "_proj_" + region + "_zplus", iBinLow, iBinUpp)
            projections.append(projectionY)
            projections[-1].SetMarkerStyle(20)
            projections[-1].SetMarkerSize(1.2) 
            projections[-1].Rebin(rebin)
            #projections[-1].Scale( 1./projections[-1].GetSumOfWeights() )
            #projections[-1].Scale( 1./nEvents )    
            ###
            if runFit:
		fitMean,fitSigma = fitHistoDoubleGaus(projections[-1])
		thresholdsPlus[type][region] = fitMean + 3*fitSigma  

            ythresholdNew = ythreshold 
            if thresholdsPlus[type].has_key(region): ythresholdNew = thresholdsPlus[type][region]
            ### 
            #projections[-1].DrawNormalized() 
            #if len(fitObjects[-1]): fitObjects[-1][0].Draw()
            projections[-1].Draw()
   
            linesProj.append(ROOT.TLine(ythresholdNew,0.,
                                        ythresholdNew,0.40*projections[-1].GetMaximum()))
            linesProj[-1].SetLineColor(2)
            linesProj[-1].SetLineStyle(2)
            linesProj[-1].SetLineWidth(4)
            linesProj[-1].Draw("SAME")

            canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zminus")
            canvasesProj.append(canvasProj)
            canvasesProj[-1].SetGridy()
            canvasesProj[-1].SetLogy()
            canvasesProj[-1].cd()

            #iBinLow = histo.GetXaxis().FindBin(-xrange[1]) + 1
            iBinLow = histo.GetXaxis().FindBin(-xrange[1])
            #iBinUpp = histo.GetXaxis().FindBin(-xrange[0])
            iBinUpp = histo.GetXaxis().FindBin(-xrange[0]) - 1
            print "Bin range for %s, %s region, eta=[%.1f,%.1f): [%d,%d]" % (type,region,-xrange[1],-xrange[0],iBinLow,iBinUpp)
            projectionY = histo.ProjectionY(histo.GetName() + "_proj_" + region + "_zminus", iBinLow, iBinUpp)
            projections.append(projectionY)
            projections[-1].SetMarkerStyle(20)
            projections[-1].SetMarkerSize(1.2)
            projections[-1].Rebin(rebin)
            #projections[-1].Scale( 1./projections[-1].GetSumOfWeights() )
            #projections[-1].Scale( 1./nEvents )
            ###
            if runFit:
		fitMean,fitSigma = fitHistoDoubleGaus(projections[-1])
		thresholdsMinus[type][region] = fitMean + 3*fitSigma  

            ythresholdNew = ythreshold 
            if thresholdsMinus[type].has_key(region): ythresholdNew = thresholdsMinus[type][region]
            ###
            #projections[-1].DrawNormalized()
            projections[-1].Draw()

            linesProj.append(ROOT.TLine(ythresholdNew,0.,
                                        ythresholdNew,0.40*projections[-1].GetMaximum()))
            linesProj[-1].SetLineColor(2)
            linesProj[-1].SetLineStyle(2)
            linesProj[-1].SetLineWidth(4)
            linesProj[-1].Draw("SAME")
 
        
	canvasProfX = ROOT.TCanvas("c_" + histo.GetName() + "_profX")
	canvasProfX.SetGridx()
	canvasProfX.SetGridy()
	canvasProfX.SetLogy()
	canvasProfX.cd() 
        profileX = histo.ProfileX(histo.GetName() + "_profX")
	profileX.SetMarkerStyle(20)
	profileX.SetMarkerSize(1.2)
	#profileX.Rebin(rebin)
	#profileX.Scale( 1./nEvents )
        profileX.Draw()

	canvasProjX = ROOT.TCanvas("c_" + histo.GetName() + "_projX")
	canvasProjX.SetGridx()
	canvasProjX.SetGridy()
	canvasProjX.SetLogy()
	canvasProjX.cd() 
        projectionX = histo.ProjectionX(histo.GetName() + "_projX")
	projectionX.SetMarkerStyle(20)
	projectionX.SetMarkerSize(1.2)
        projectionX.Draw()

        histoSumEnergyVsEta = file.Get("%s/%s" % (selection,histoNamesSumEnergyVsEta[type]))
        canvasSumEnergyVsEta = ROOT.TCanvas("c_" + histoSumEnergyVsEta.GetName())
        canvasSumEnergyVsEta.SetGridy()
        canvasSumEnergyVsEta.cd() 
        histoSumEnergyVsEta.SetMarkerSize(1.4)
        histoSumEnergyVsEta.GetXaxis().SetTitle("#eta")
        histoSumEnergyVsEta.GetYaxis().SetTitle("E_{PF}(#eta) / N_{Ev} (GeV)")
        #histoSumEnergyVsEta.SetStats(0)
        histoSumEnergyVsEta.Scale( 1./nEvents )
        histoSumEnergyVsEta.Draw()

        canvases.append(canvas)
        canvases.append(canvasSumEnergyVsEta)
        canvases.append(canvasProfX)
        canvases.append(canvasProjX)
        canvases.append(canvasesProj)
        histos.append(histo)
        histos.append(histoSumEnergyVsEta)
        histos.append(profileX)
        histos.append(projectionX)
        for proj in projections: histos.append(proj)
        objects.append(fitObjects)
        objects.append(lines)
        objects.append(linesProj)

    ROOT.TH1.AddDirectory(AddDirectoryStatus_)

    for type in particleTypes:
        if not thresholdsPlus.has_key(type): continue
        for region in thresholdsPlus[type]:
            xrange = regions[region]
            ythreshold = 0.
            if thresholds[type].has_key(region): ythreshold = thresholds[type][region]
            ythresholdNew = thresholdsPlus[type][region]
            print "New threshold for %s, %s region, eta=[%.1f,%.1f): %.2f --> %.2f" % (type,region,xrange[0],xrange[1],ythreshold,ythresholdNew)

    for type in particleTypes:
        if not thresholdsMinus.has_key(type): continue
        for region in thresholdsMinus[type]:
            xrange = regions[region]
            ythreshold = 0.
            if thresholds[type].has_key(region): ythreshold = thresholds[type][region]
            ythresholdNew = thresholdsMinus[type][region]
            print "New threshold for %s, %s region, eta=(%.1f,%.1f]: %.2f --> %.2f" % (type,region,-xrange[1],xrange[0],ythreshold,ythresholdNew)

    if outputFileName:
        outputFile = ROOT.TFile(outputFileName,'recreate')
        outputFile.cd()
        for item in histos: item.Write()
        return 0
    else:       
        return (canvases,histos,objects)   
