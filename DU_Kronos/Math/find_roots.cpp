/*Description: Finds the real roots to a polynomial.
Usage: polyRoots({{2,-3,1}},-1e9,1e9) // solve xˆ2-3x+2 = 0
Time: O(n^2log(1/ϵ)))*/
struct Poly {
    vector<double> a;
    double operator()(double x) const {
        double val = 0;
        for (int i = a.size(); i--;) (val *= x) += a[i];
        return val;
    }
    void diff() {
        for (int i = 1; i < a.size(); ++i) a[i - 1] = i * a[i];
        a.pop_back();
    }
    void divroot(double x0) {
        double b = a.back(), c;
        a.back() = 0;
        for (int i = a.size() - 1; i--;) c = a[i], a[i] = a[i + 1] * x0 + b, b = c;
        a.pop_back();
    }
};
vector<double> polyRoots(Poly p, double xmin = -1e9, double xmax = 1e9) {
    if (p.a.size() == 2) {
        return {-p.a[0] / p.a[1]};
    }
    vector<double> ret;
    Poly der = p;
    der.diff();
    auto dr = polyRoots(der, xmin, xmax);
    dr.push_back(xmin - 1);
    dr.push_back(xmax + 1);
    sort(dr.begin(), dr.end());
    for (int i = 0; i < dr.size() - 1; ++i) {
        double l = dr[i], h = dr[i + 1];
        bool sign = p(l) > 0;
        if (sign ^ (p(h) > 0)) {
            for (int it = 0; it < 60; ++it) {  // while (h-l > 1e-8)
                double m = (l + h) / 2, f = p(m);
                if ((f <= 0) ^ sign)
                    l = m;
                else
                    h = m;
            }
            ret.push_back((l + h) / 2);
        }
    }
    return ret;
}