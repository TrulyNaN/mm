#!/usr/bin/env python3

import argparse, json, os, signal, time, colorama, multiprocessing

colorama.init()

EXTRACTED_ASSETS_NAMEFILE = ".extracted-assets.json"

def SignalHandler(sig, frame):
    print(f'Signal {sig} received. Aborting...')
    mainAbort.set()
    # Don't exit immediately to update the extracted assets file.

def ExtractFile(xmlPath, outputPath, outputSourcePath):
    if globalAbort.is_set():
        # Don't extract if another file wasn't extracted properly.
        return

    execStr = f"tools/ZAPD/ZAPD.out e -eh -i {xmlPath} -b baserom/ -o {outputPath} -osf {outputSourcePath} -gsf 1 -rconf tools/ZAPDConfigs/MM/Config.xml {ZAPDArgs}"

    if globalUnaccounted:
        execStr += " -Wunaccounted"

    print(execStr)
    exitValue = os.system(execStr)
    if exitValue != 0:
        globalAbort.set()
        print("\n")
        print("Error when extracting from file " + xmlPath, file=os.sys.stderr)
        print("Aborting...", file=os.sys.stderr)
        print("\n")

def ExtractFunc(fullPath):
    *pathList, xmlName = fullPath.split(os.sep)
    objectName = os.path.splitext(xmlName)[0]

    outPath = os.path.join("assets", *pathList[2:-1], objectName)
    basromPath = os.path.join("baserom", "assets", *pathList[2:-1])
    outSourcePath = outPath

    ## MM doesn't have _scene prefixed files, so this check is not necessary.
    ## This _may_ change in the future, so I wont delete this for now.
    #isScene = fullPath.startswith("assets/xml/scenes/")
    #if isScene:
    #    objectName += "_scene"

    if not globalForce:
        cFile = os.path.join(outPath, objectName + ".c")
        hFile = os.path.join(outPath, objectName + ".h")
        if not checkTouchedFile(fullPath, cFile) and not checkTouchedFile(fullPath, hFile):
            return

    ExtractFile(fullPath, basromPath, outPath, outSourcePath)

def initializeWorker(force: bool, abort, unaccounted: bool):
    global globalForce
    global globalAbort
    global globalUnaccounted
    globalForce = force
    globalAbort = abort
    globalUnaccounted = unaccounted

def main():
    parser = argparse.ArgumentParser(description="baserom asset extractor")
    parser.add_argument("-s", "--single", help="asset path relative to assets/, e.g. objects/gameplay_keep")
    parser.add_argument("-f", "--force", help="Force the extraction of every xml instead of checking the touched ones.", action="store_true")
    parser.add_argument("-j", "--jobs", help="Number of cpu cores to extract with.")
    parser.add_argument("-u", "--unaccounted", help="Enables ZAPD unaccounted detector warning system.", action="store_true")
    parser.add_argument("-Z", help="Pass the argument on to ZAPD, e.g. `-ZWunaccounted` to warn about unaccounted blocks in XMLs. Each argument should be passed separately, *without* the leading dash.", metavar="ZAPD_ARG", action="append")
    args = parser.parse_args()

    global ZAPDArgs
    ZAPDArgs = ""
    if args.Z is not None:
        badZAPDArg = False
        for i in range(len(args.Z)):
            z = args.Z[i]
            if z[0] == '-':
                print(f"{colorama.Fore.LIGHTRED_EX}error{colorama.Fore.RESET}: argument \"{z}\" starts with \"-\", which is not supported.", file=os.sys.stderr)
                badZAPDArg = True
            else:
                args.Z[i] = "-" + z

        if badZAPDArg:
            exit(1)

        ZAPDArgs = " ".join(args.Z)
        print("Using extra ZAPD arguments: " + ZAPDArgs)

    global mainAbort
    mainAbort = multiprocessing.Event()
    manager = multiprocessing.Manager()
    signal.signal(signal.SIGINT, SignalHandler)

    extractedAssetsTracker = manager.dict()
    if os.path.exists(EXTRACTED_ASSETS_NAMEFILE) and not args.force:
        with open(EXTRACTED_ASSETS_NAMEFILE, encoding='utf-8') as f:
            extractedAssetsTracker.update(json.load(f, object_hook=manager.dict))

    asset_path = args.single
    if asset_path is not None:
        # Always force if -s is used.
        initializeWorker(True, abort, args.unaccounted)
        fullPath = os.path.join("assets", "xml", asset_path + ".xml")
        if not os.path.exists(fullPath):
            print(f"Error. File {fullPath} does not exist.", file=os.sys.stderr)
            exit(1)

        initializeWorker(mainAbort, args.unaccounted, extractedAssetsTracker, manager)
        # Always extract if -s is used.
        if fullPath in extractedAssetsTracker:
            del extractedAssetsTracker[fullPath]
        ExtractFunc(fullPath)
    else:
        xmlFiles = []
        for currentPath, folders, files in os.walk(os.path.join("assets", "xml")):
            for file in files:
                fullPath = os.path.join(currentPath, file)
                if file.endswith(".xml"):
                    xmlFiles.append(fullPath)

        try:
            numCores = int(args.jobs or 0)
            if numCores <= 0:
                numCores = 1
            print("Extracting assets with " + str(numCores) + " CPU core" + ("s" if numCores > 1 else "") + ".")
            with multiprocessing.get_context("fork").Pool(numCores,  initializer=initializeWorker, initargs=(mainAbort, args.unaccounted, extractedAssetsTracker, manager)) as p:
                p.map(ExtractFunc, xmlFiles)
        except (multiprocessing.ProcessError, TypeError):
            print("Warning: Multiprocessing exception ocurred.", file=os.sys.stderr)
            print("Disabling mutliprocessing.", file=os.sys.stderr)

            initializeWorker(mainAbort, args.unaccounted, extractedAssetsTracker, manager)
            for singlePath in xmlFiles:
                ExtractFunc(singlePath)

    if abort.is_set():
        exit(1)

if __name__ == "__main__":
    main()
