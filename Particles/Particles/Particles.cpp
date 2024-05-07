#include "Particle.h"

using namespace std;

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
{
    m_radiansPerSec = ((float)rand() / RAND_MAX) * M_PI; // intialize
    m_numPoints = numPoints;
    m_ttl = TTL;

    m_cartesianPlane.setCenter(0, 0); //initialize cartesian plane
    m_cartesianPlane.setSize(target.getSize().x, -1.0 * target.getSize().y);// initialize height and width ot the size of renderwindow and invert y axis
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    m_vx = rand() % 401 + 100; // random pixel velocity between 100 and 500
    m_vy = rand() % 401 + 100;
    if (rand() % 2 != 0) // m_vx random positive or negative
    {
        m_vx *= -1;
    }
    m_color1 = Color(rand() % 256, rand() % 256, rand() % 256); //random colors
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

    double theta = ((float)rand() / RAND_MAX) * M_PI / 2; // initialize theta between 0 and pi/2
    double dTheta = 2 * M_PI / (numPoints - 1); //amt we rotate per vertex. overlap so we dont leave an open edge

    for (int j = 0; j < numPoints; j++)
    {
        float r = rand() % 61 + 20;
        float dx = r * cos(theta);
        float dy = r * sin(theta);

        m_A(0, j) = m_centerCoordinate.x + dx; // assing cartiesian coordainte of the newly generated vertex
        m_A(1, j) = m_centerCoordinate.y + dy;

        theta += dTheta; // increment theta by dtheta
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    // construct vertex array named lines
    VertexArray lines(TriangleFan, m_numPoints + 1);
    //declare a local vector2f named center. stores lcoation on the monitor of particle center
    Vector2f center = static_cast<Vector2f>(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));
    // assign lines[0]
    lines[0].position = center;
    lines[0].color = m_color1;
    //loop for particle pixel coordinates
    for (int j = 1; j <= m_numPoints; j++)
    {
        Vector2f point(m_A(0, j - 1), m_A(1, j - 1));
        Vector2i pixelPoint = target.mapCoordsToPixel(point, m_cartesianPlane);
        lines[j].position = Vector2f(static_cast<float>(pixelPoint.x), static_cast<float>(pixelPoint.y));
        lines[j].color = m_color2;
    }
    // when loop is finished, draw vertex array
    target.draw(lines);
}

void Particle::update(float dt)
{
    // subtract dt from mttl
    m_ttl = m_ttl - dt;
    // call rotate with an angle of dt*radsPerSec
    rotate(dt * m_radiansPerSec);
    // call scale with SCALE
    scale(SCALE);
    // declare float dx and dy, prefrom the math in the instructions
    float dx = m_vx * dt;
    m_vy = m_vy - (G * dt);
    float dy = m_vy * dt;
    translate(dx, dy);
}

void Particle::rotate(double theta)
{
    // store value in a vector2f temp
    Vector2f temp = m_centerCoordinate;
    // shift particle center to the origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    // construct rotational matrix r with specified rotation theta  
    RotationMatrix R(theta);
    m_A = R * m_A;
    translate(temp.x, temp.y);
}

void Particle::scale(double c)
{
    // store center coordinate value in temporary location
    Vector2f temp = m_centerCoordinate;
    ScalingMatrix S(c);
    // shift particle center back to origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    m_A = S * m_A;
    translate(temp.x, temp.y);

}
void Particle::translate(double xShift, double yShift)
{
    TranslationMatrix T(xShift, yShift, m_numPoints);
    m_A = T + m_A;
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

// below this point is all unit tests

bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }


    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}