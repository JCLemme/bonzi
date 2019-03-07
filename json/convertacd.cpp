#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

// Convertacd
// Generates a JSON file from a Microsoft Agent
// by John Lemme (jclemme at proportionallabs dot com)

typedef struct BranchOption
{
    int location;
    int probability;
} BranchOption;

typedef struct Mouth
{
    int image;
    std::string type;
} Mouth;

typedef struct Frame
{
    int duration;
    int exit;
    std::string effect;
    
    std::vector<BranchOption> branches;
    
    std::vector<int> images;
    std::vector<Mouth> mouth;
} Frame;

typedef struct Animation
{
    std::string areturn;
    int transitionType;
    std::vector<Frame> frames;
} Animation;
    
std::string getQuoted(std::string src)
{
    std::string::size_type first = src.find("\"");
    first++;
    std::string::size_type second = src.find("\"", first);
    
    return src.substr(first, second-first); 
}

int getIndex(std::string src)
{
    std::string::size_type first = src.find("\\");
    first++;
    std::string::size_type second = src.find(".", first);
    
    return stoi(src.substr(first, second-first));
}

std::string getStrIndex(std::string src)
{
    std::string::size_type first = src.find("\\");
    first++;
    std::string::size_type second = src.find(".", first);
    
    return src.substr(first, second-first);
}
        
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "No agent specified - aborting\n";
        return -1;
    }
    
    // Load dat agent file, looking for useful blocks
    std::vector<std::string> agentFile;
    std::vector<int> animationDefs, stateDefs;
    
    std::ifstream agentFileStream(argv[1]);
    std::string tempstr; 
    int l = 0;
    
    while(std::getline(agentFileStream, tempstr))
    {
        agentFile.push_back(tempstr);
        
        if(tempstr.find("DefineAnimation") != std::string::npos)
            animationDefs.push_back(l);
        else if(tempstr.find("DefineState") != std::string::npos)
            stateDefs.push_back(l);
        
        l++;
    }
    
    // Look for animations
    std::map<std::string, Animation> animations;
    std::vector<std::string> imagesToLoad;
    
    for(int i=0;i<animationDefs.size();i++)
    {
        // We'll need that index
        l = animationDefs[i];
        
        // Get the animation name
        std::string name = getQuoted(agentFile[l]);
        std::cout << "Animation name: " << name << "\n";
        
        l++;
        
        // Loop through for frames, associated data
        bool parsing = true, onFrame = false;
        Animation anim;
        Frame frame;
        
        while(parsing)
        {
            std::string line = agentFile[l];
            
            if(!onFrame)
            {
                if(line.find("DefineFrame") != std::string::npos)
                {
                    onFrame = true;
                    frame.images.clear();
                    frame.mouth.clear();
                    frame.branches.clear();
                    
                    frame.exit = -1;
                    frame.duration = 10;
                    frame.effect = "";
                    
                    std::cout << "  Found new frame...\n";
                }
                else if(line.find("TransitionType") != std::string::npos)
                {
                    std::string type = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
                    anim.transitionType = stoi(type);
                    std::cout << "  Transition type is " << type << "\n";

                }
                else if(line.find("Return") != std::string::npos)
                {
                    std::string type = getQuoted(agentFile[l]);
                    anim.areturn = type;
                    std::cout << "  Return is " << type << "\n";

                }
                else if(line.find("EndAnimation") != std::string::npos)
                {
                    parsing = false;
                }
            }
            else
            {
                if(line.find("EndFrame") != std::string::npos)
                {
                    onFrame = false;
                    anim.frames.push_back(frame);

                    std::cout << "  Frame ended\n";
                }
                else
                {
                    if(line.find("DefineImage") != std::string::npos)
                    {
                        l++;
                        std::string filename = getQuoted(agentFile[l]);
                        
                        if(std::find(imagesToLoad.begin(), imagesToLoad.end(), filename) == imagesToLoad.end())
                            imagesToLoad.push_back(filename);
                            
                        frame.images.insert(frame.images.begin(), getIndex(filename));
                        l++;
                        std::cout << "    Found still frame with image \"" << filename << "\"\n";
                    }
                    else if(line.find("DefineMouth") != std::string::npos)
                    {
                        Mouth mouth;
                        l++;
                        std::string mouthType = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
                        mouthType.erase(std::remove(mouthType.begin(), mouthType.end(), '\r'), mouthType.end());    // For some reason this includes a carriage return, so axe it
                        mouth.type = mouthType;
                        l++;
                        std::string filename = getQuoted(agentFile[l]);
                        
                        if(std::find(imagesToLoad.begin(), imagesToLoad.end(), filename) == imagesToLoad.end())
                            imagesToLoad.push_back(filename);
                            
                        mouth.image = getIndex(filename);
                        l++;
                        frame.mouth.insert(frame.mouth.begin(), mouth);
                        std::cout << "    Found mouth with image \"" << mouth.image << "\" and type \"" << mouth.type << "\"\n";
                    }
                    else if(line.find("DefineBranching") != std::string::npos)
                    {
                        BranchOption branch;
                        l++;
                        
                        while(agentFile[l].find("EndBranching") == std::string::npos)
                        {
                            std::string location = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
                            branch.location = stoi(location);

                            l++;
                            
                            std::string probability = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
                            branch.probability = stoi(probability);
                            
                            l++;
                            
                            frame.branches.push_back(branch);
                            std::cout << "Found branch to " << branch.location << " with probability of " << branch.probability << "%\n";
                        }
                        
                        std::cout << "    Grabbing branch data\n";
                    }
                    else if(line.find("ExitBranch") != std::string::npos)
                    {
                        std::string exitBranch = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
                        frame.exit = stoi(exitBranch);
                        std::cout << "    Found exitbranch: " << frame.exit << "\n";
                    }
                    else if(line.find("Duration") != std::string::npos)
                    {
                        std::string duration = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
                        frame.duration = stoi(duration);
                        std::cout << "    Found duration: " << frame.duration << "\n";
                    }
                    else if(line.find("SoundEffect") != std::string::npos)
                    {
                        std::string effect = getStrIndex(agentFile[l]);
                        frame.effect = effect;
                        std::cout << "    Found soundeffect: " << frame.effect << "\n";
                    }
                    else 
                    {
                        std::cout << "    Failed to find token for line " << l << ": \"" << agentFile[l] << "\"\n";
                    }
                }
            }
            
            l++;
        }
        
        // And store
        animations[name] = anim;
        
        std::cout << "Animation ended\n\n";
    }
    
    std::cout << "Finished loading animations.\n";
    
    std::vector<std::string> json;
    
    json.push_back("{");
    json.push_back("    \"animations\": [");
    
    for(std::map<std::string, Animation>::iterator it=animations.begin(); it!=animations.end(); ++it)
    {
        json.push_back("        {");
        json.push_back("            \"name\": \"" + it->first + "\",");
        json.push_back("            \"return\": \"" + it->second.areturn + "\",");
        json.push_back("            \"transitionType\": " + std::to_string(it->second.transitionType) + ",");
        json.push_back("            \"frames\": [");
        
        for(int i=0;i<it->second.frames.size();i++)
        {
            json.push_back("                {");
            json.push_back("                    \"duration\": " + std::to_string(it->second.frames[i].duration) + ",");
            json.push_back("                    \"exit\": " + std::to_string(it->second.frames[i].exit) + ",");
            json.push_back("                    \"effect\": \"" + it->second.frames[i].effect + "\",");
            
            json.push_back("                    \"branches\": [");
            for(int j=0;j<it->second.frames[i].branches.size();j++)
            {
                json.push_back("                        {");
                json.push_back("                            \"location\": " + std::to_string(it->second.frames[i].branches[j].location) + ",");
                json.push_back("                            \"probability\": " + std::to_string(it->second.frames[i].branches[j].probability));
                if(j == it->second.frames[i].branches.size()-1)
                    json.push_back("                }");
                else
                    json.push_back("                },");
            }
            json.push_back("                    ],");

            json.push_back("                    \"images\": [");
            for(int j=0;j<it->second.frames[i].images.size();j++)
            {
                if(j == it->second.frames[i].images.size()-1)
                    json.push_back("                        " + std::to_string(it->second.frames[i].images[j]));
                else
                    json.push_back("                        " + std::to_string(it->second.frames[i].images[j]) + ",");
            }
            json.push_back("                    ],");

            json.push_back("                    \"mouth\": [");
            for(int j=0;j<it->second.frames[i].mouth.size();j++)
            {
                json.push_back("                        {");
                json.push_back("                            \"image\": " + std::to_string(it->second.frames[i].mouth[j].image) + ",");
                json.push_back("                            \"type\": \"" + it->second.frames[i].mouth[j].type + "\"");
                if(j == it->second.frames[i].mouth.size()-1)
                    json.push_back("                }");
                else
                    json.push_back("                },");
            }
            json.push_back("                    ]");

            if(i == it->second.frames.size()-1)
                json.push_back("                }");
            else
                json.push_back("                },");
        }
        
        json.push_back("            ]");
        json.push_back("        },");
    }
    
    json.push_back("    ]");
    json.push_back("}");
    
    std::ofstream outFile("./Bonzi.json");
    std::ostream_iterator<std::string> outIt(outFile, "\n");
    std::copy(json.begin(), json.end(), outIt);
    
    return 0;
}
