#include "simulator.h"

Simulator::Simulator(int w, int h, int nL, int nP, float fNoise, float tNoise, float sNoise) : w(w), h(h), nLandmarks(nL), nParticles(nP), fNoise(fNoise), tNoise(tNoise), sNoise(sNoise)
{
    cv::namedWindow("Particle Filter", 0);

    robot = Robot(cv::Point(w/2, h/2), 0, fNoise, tNoise, sNoise);
    filter = ParticleFilter(nParticles);
    
    initLandmarks();
    initParticles();
}

void Simulator::initLandmarks()
{
    srand(time(NULL));
    landmarks.clear();
    for (int i = 0; i < nLandmarks; i++)
    {
        float x = w * (double)std::rand() / RAND_MAX;
        float y = h * (double)std::rand() / RAND_MAX;
        landmarks.push_back(cv::Point(x, y));
    }
}

void Simulator::initParticles()
{
    particles.clear();
    for (int i = 0; i < nParticles; i++)
    {
        float x = w * (double)std::rand() / RAND_MAX;
        float y = h * (double)std::rand() / RAND_MAX;
        float yaw = 2 * M_PI * (double)std::rand() / RAND_MAX;
        Robot p = Robot(cv::Point(x, y), yaw, fNoise, tNoise, sNoise);
        particles.push_back(p); 
    }
}

void Simulator::drawScene(cv::Mat& canvas)
{
    canvas.setTo(cv::Scalar(255, 255, 255));
    cv::circle(canvas, robot.pos, 10, cv::Scalar(0, 0, 0), -1);
    for (auto& landmark : landmarks)
    {
        cv::circle(canvas, landmark, 5, cv::Scalar(0, 0, 255), -1);
        cv::line(canvas, landmark, robot.pos, cv::Scalar(255, 0, 255), 1, cv::LINE_AA);
    }

    for (auto& particle : particles)
    {
        cv::Scalar color = cv::Scalar(std::rand() % 255, std::rand() % 255, std::rand() % 255);
        cv::circle(canvas, particle.pos, 1, color, -1);
    }
}

int Simulator::run()
{
    cv::Mat canvas(h, w, CV_8UC3);
    vector<float> weights(nParticles, 0);
    vector<float> Z(nLandmarks, 0);
    Robot p;
    //cv::VideoWriter video("part.mp4", CV_FOURCC('F','M','P','4'), 100, cv::Size(w, h));
    while (true)
    {
        float turn = 2 * M_PI * (double)std::rand() / RAND_MAX;
        float forward = 0.5;
        
        robot.move(turn, forward);
        robot.sense(landmarks, Z);
        
        for (int i = 0; i < nParticles; i++)
        {
            p = particles[i].move(turn, forward);
            particles[i] = p;
            weights[i] = p.measProb(landmarks, Z);
        }
        
        float norm = 0.0;
        for (const auto& weight : weights)
        {
            norm += weight;
        }
        for (auto& weight : weights)
        {
            weight /= norm;
        }
        
        int maxIndex = filter.resample(weights, particles);
        
        std::cout << "Actual robot position:    [" << robot.pos.x << ", " << robot.pos.y << "]\n"
                     "Estimated robot position: " << particles[maxIndex] << std::endl;
        
        drawScene(canvas);
        cv::imshow("Particle Filter", canvas);
        //video.write(canvas);
        if (cv::waitKey(1) == 27)
            break;
    }
    //video.release();
    cv::destroyAllWindows();
    return 0; 
}
