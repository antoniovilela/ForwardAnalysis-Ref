import ROOT

def plot(fileName,selection="pFlowAnalysisNoCollNoVtx"):

    ROOT.TH1.AddDirectory(False)
    
    file = ROOT.TFile(fileName,'READ')

    """
    h_energyVsEtaAllTypes
    h_energyVsEtaUndefined
    h_energyVsEtaChargedHadron
    h_energyVsEtaElectron
    h_energyVsEtaMuon
    h_energyVsEtaGamma
    h_energyVsEtaNeutralHadron
    h_energyVsEtaHadronHF
    h_energyVsEtaEGammaHF
    """

    particleTypes = ("all","gamma","hadronNeutral","hadronHF","emHF")
    histoNames = {}
    histoNames["all"] = "energyVsEtaAllTypes" 
    histoNames["gamma"] = "energyVsEtaGamma"
    histoNames["hadronNeutral"] = "energyVsEtaNeutralHadron"
    histoNames["hadronHF"] = "energyVsEtaHadronHF"
    histoNames["emHF"] = "energyVsEtaEGammaHF"
    regions = {}
    regions["Barrel"] = (0.,1.4)
    regions["Endcap"] = (1.4,2.6) 
    regions["Transition"] = (2.6,3.2)
    regions["Forward"] = (3.2,5.205)
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

    canvases = []
    histos = []
    objects = []
    for type in particleTypes:
        histo = file.Get("%s/%s" % (selection,histoNames[type]))
        canvas = ROOT.TCanvas("c_" + histo.GetName())
        #canvas.SetGridy()
        canvas.cd() 
        
        histo.SetMarkerSize(0.1)
        #histo.SetMarkerColor(4)
        histo.GetXaxis().SetTitle("#eta")
        histo.GetYaxis().SetTitle("Energy (GeV)")
        histo.SetStats(0)
        histo.Draw()

        lines = []
        canvasesProj = []         
        projections = []
        linesProj = []
        for region in thresholds[type]:
            xrange = regions[region]
            ythreshold = thresholds[type][region]

            canvas.cd()
            lines.append(ROOT.TLine(xrange[0],ythreshold,xrange[1],ythreshold))
            lines[-1].SetLineColor(2)
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(5)
            lines[-1].Draw("SAME")
            lines.append(ROOT.TLine(-xrange[1],ythreshold,-xrange[0],ythreshold))
            lines[-1].SetLineColor(2) 
            lines[-1].SetLineStyle(2)
            lines[-1].SetLineWidth(4)
            lines[-1].Draw("SAME")

            # Projections
            canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zplus")
            canvasesProj.append(canvasProj)
            canvasesProj[-1].SetGridy()
            canvasesProj[-1].SetLogy()
            canvasesProj[-1].cd()

            iBinLow = histo.GetXaxis().FindBin(xrange[0])
            iBinUpp = histo.GetXaxis().FindBin(xrange[1]) - 1
            print "Bins range for %s, %s region, eta=[%.1f,%.1f): [%d,%d]" % (type,region,xrange[0],xrange[1],iBinLow,iBinUpp)
            projectionY = histo.ProjectionY(histo.GetName() + "_proj_" + region + "_zplus", iBinLow, iBinUpp)
            projections.append(projectionY)
            projections[-1].DrawNormalized() 
   
            linesProj.append(ROOT.TLine(ythreshold,0.,
                                        ythreshold,0.15))
            linesProj[-1].SetLineColor(2)
            linesProj[-1].SetLineStyle(2)
            linesProj[-1].SetLineWidth(4)
            linesProj[-1].Draw("SAME")

            canvasProj = ROOT.TCanvas("c_" + histo.GetName() + "_proj_" + region + "_zminus")
            canvasesProj.append(canvasProj)
            canvasesProj[-1].SetGridy()
            canvasesProj[-1].SetLogy()
            canvasesProj[-1].cd()

            iBinLow = histo.GetXaxis().FindBin(-xrange[1]) + 1
            iBinUpp = histo.GetXaxis().FindBin(-xrange[0])
            print "Bins range for %s, %s region, eta=(%.1f,%.1f]: [%d,%d]" % (type,region,-xrange[1],-xrange[0],iBinLow,iBinUpp)
            projectionY = histo.ProjectionY(histo.GetName() + "_proj_" + region + "_zminus", iBinLow, iBinUpp)
            projections.append(projectionY)
            projections[-1].DrawNormalized()

            linesProj.append(ROOT.TLine(ythreshold,0.,
                                        ythreshold,0.15))
            linesProj[-1].SetLineColor(2)
            linesProj[-1].SetLineStyle(2)
            linesProj[-1].SetLineWidth(4)
            linesProj[-1].Draw("SAME")     
 
        canvases.append(canvas)
        canvases.append(canvasesProj)
        histos.append(histo)
        histos.append(projections)
        objects.append(lines)
        objects.append(linesProj)

    return (canvases,histos,objects)   
