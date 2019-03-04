#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

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
    std::vector<Frame> frames;
} Animation;

SDL_Renderer* renderer;
SDL_Texture* background;
SDL_Texture* bubble;
TTF_Font* speakFont;

std::map<int, SDL_Texture*> images;
    
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

void showAnimation(Animation anim, bool reverse, bool mouth)
{    
    int animStart = 0, animEnd = anim.frames.size(), animMod = 1;
    
    SDL_Rect clear = {0, 100, 200, 160};
    SDL_Rect buddy = {0, 100, 200, 160};
            
    if(reverse)
    {
        animStart = anim.frames.size()-1;
        animEnd = -1;
        animMod = -1;
    }
    
    for(int f=animStart;f!=animEnd;f+=animMod)
    {
        std::cout << "Frame " << f << ":\n";
 
        if(anim.frames[f].mouth.size() > 0 && mouth)
        {
            int mouthStart = 0, mouthEnd = anim.frames[f].mouth.size(), mouthMod = 1;
            
            if(reverse)
            {
                mouthStart = anim.frames[f].mouth.size()-1;
                mouthEnd = -1;
                mouthMod = -1;
            }
            
            for(int m=mouthStart;m!=mouthEnd;m+=mouthMod)
            {
                if(anim.frames[f].images.size() != 0)
                    SDL_RenderCopy(renderer, background, &clear, &buddy);
                
                for(int i=0;i<anim.frames[f].images.size();i++)
                {
                    std::cout << "  Displaying image " << anim.frames[f].images[i] << "\n";
                    SDL_RenderCopy(renderer, images[anim.frames[f].images[i]], NULL, &buddy);
                }
                
                SDL_RenderCopy(renderer, images[anim.frames[f].mouth[m].image], NULL, &buddy);
            
                SDL_RenderPresent(renderer);
                std::cout << "Pausing for duration " << anim.frames[f].duration << "\n";
                SDL_Delay(anim.frames[f].duration+40);
            }
        }
        else
        {
            if(anim.frames[f].images.size() != 0)
                SDL_RenderCopy(renderer, background, &clear, &buddy);
            
            for(int i=0;i<anim.frames[f].images.size();i++)
            {
                std::cout << "  Displaying image " << anim.frames[f].images[i] << "\n";
                SDL_RenderCopy(renderer, images[anim.frames[f].images[i]], NULL, &buddy);
            }
            
            SDL_RenderPresent(renderer);
            std::cout << "Pausing for duration " << anim.frames[f].duration << "\n";
            SDL_Delay(anim.frames[f].duration+40);
        }
    }
}

void showBubble(char* text, bool show)
{
    SDL_Rect clear = {0, 0, 200, 100};
    SDL_Rect talking = {4, 4, 200, 100};
    SDL_Rect bubblebox = {0, 0, 200, 100};
    
    SDL_RenderCopy(renderer, background, &clear, &bubblebox);
    
    if(show)
    {
        SDL_Texture* speech = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended_Wrapped(speakFont, text, {0, 0, 0, 255}, 152));
        SDL_QueryTexture(speech, NULL, NULL, &talking.w, &talking.h);

        SDL_RenderCopy(renderer, bubble, NULL, &bubblebox);
        SDL_RenderCopy(renderer, speech, NULL, &talking);
    }
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
                    std::cout << "  Found new frame...\n";
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
                    frame.images.clear();
                    frame.mouth.clear();
                    frame.branches.clear();
                    
                    frame.exit = 0;
                    frame.duration = 10;
                    frame.effect = "";
                    
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
                            
                        frame.images.push_back(getIndex(filename));
                        l++;
                        std::cout << "    Found still frame with image \"" << filename << "\"\n";
                    }
                    else if(line.find("DefineMouth") != std::string::npos)
                    {
                        Mouth mouth;
                        l++;
                        std::string mouthType = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
                        mouth.type = mouthType;
                        l++;
                        std::string filename = getQuoted(agentFile[l]);
                        
                        if(std::find(imagesToLoad.begin(), imagesToLoad.end(), filename) == imagesToLoad.end())
                            imagesToLoad.push_back(filename);
                            
                        mouth.image = getIndex(filename);
                        l++;
                        frame.mouth.push_back(mouth);
                        std::cout << "    Found mouth with image \"" << mouth.image << "\" and type \"" << mouth.type << "\"\n";
                    }
                    else if(line.find("DefineBranching") != std::string::npos)
                    {
                        while(agentFile[l].find("EndBranching") == std::string::npos)
                        {
                            l++;
                        }
                        
                        std::cout << "    Skipping past branching section\n";
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
                        std::string effect = agentFile[l].substr(agentFile[l].find('=')+2, std::string::npos);
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
    
    // Load all those images
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    
    SDL_Window* window = SDL_CreateWindow("Bonzi Buddy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 260, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
 
    speakFont = TTF_OpenFont("msss.ttf", 12);
    
    for(int p=0;p<imagesToLoad.size();p++)
    {
        std::cout << "Loading image \"" << std::string("bonzi_data/Images/" + getStrIndex(imagesToLoad[p]) + ".bmp.png") << "\"\n";
        images[getIndex(imagesToLoad[p])] = (SDL_CreateTextureFromSurface(renderer, IMG_Load(std::string("bonzi_data/Images/" + getStrIndex(imagesToLoad[p]) + ".bmp.png").c_str())));
        
        if(images[getIndex(imagesToLoad[p])] == NULL)
            std::cout << "ERR\n";
    }
    
    bool running = true;
    SDL_Event event;
            
    background = SDL_CreateTextureFromSurface(renderer, IMG_Load("bg.png"));
    bubble = SDL_CreateTextureFromSurface(renderer, IMG_Load("bubble.png"));
    SDL_RenderCopy(renderer, background, NULL, NULL);
        
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
        }
        
        SDL_Rect buddy = {0, 100, 200, 160};
        
        SDL_RenderCopy(renderer, background, &buddy, &buddy);
        SDL_RenderCopy(renderer, images[0], NULL, &buddy);
        SDL_RenderPresent(renderer);
        
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
        }
        
        bool reverse = false;
        
        std::cout << "Enter animation: \n";
        std::string choice;
        std::getline(std::cin, choice);
        
        if(choice == "")
            choice = "Decline";
            
        if(choice[0] == '!')
        {
            choice = choice.substr(1, std::string::npos);
            reverse = true;
        }
        
        if(!animations.count(choice))
            choice = "Decline";
            
        Animation anim = animations[choice];
        
        showBubble("Hiya", true);
        showAnimation(anim, reverse, true);
    }
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
