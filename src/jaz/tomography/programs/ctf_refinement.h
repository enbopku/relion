#ifndef CTF_REFINE_PROGRAM_H
#define CTF_REFINE_PROGRAM_H

#include <string>
#include <src/jaz/math/t_complex.h>
#include <src/jaz/image/buffered_image.h>
#include <src/jaz/optics/aberration_fit.h>
#include <src/jaz/tomography/particle_set.h>
#include <src/jaz/gravis/t4Matrix.h>
#include <src/jaz/optimization/optimization.h>
#include <src/jaz/optics/aberration_fit.h>
#include <vector>

#include "refinement.h"

class CTF;

class CtfRefinementProgram : public RefinementProgram
{
	public:
		
		CtfRefinementProgram(int argc, char *argv[]);
			
			bool do_refine_defocus, do_refine_scale, do_refine_aberrations,
				do_fit_Beer_Lambert_per_tomo, do_fit_Beer_Lambert_globally,
				do_even_aberrations, do_odd_aberrations;

			int deltaSteps, n_even, n_odd;
			double minDelta, maxDelta, lambda_reg;
			
		void run();
		
		
	private:

		void processTomograms(
				int first_t,
				int last_t,
				const AberrationsCache& aberrationsCache,
				int verbosity);

		
		void refineDefocus(
				int t,
				Tomogram& tomogram,
				const AberrationsCache& aberrationsCache,
				const BufferedImage<float>& freqWeights,
				const BufferedImage<float>& doseWeights);

		void updateScale(
				int t,
				Tomogram& tomogram,
				const AberrationsCache& aberrationsCache,
				const BufferedImage<float>& freqWeights,
				const BufferedImage<float>& doseWeights);

		void updateAberrations(
				int t,
				const Tomogram& tomogram,
				const AberrationsCache& aberrationsCache,
				const BufferedImage<float>& freqWeights,
				const BufferedImage<float>& doseWeights);


		void fitGlobalScale();

		void fitAberrations();



		BufferedImage<double> evaluateDefocusRange(
				const BufferedImage<aberration::EvenData>& evenData,
				double pixelSize,
				const std::vector<CTF>& ctfs,
				double minDefocus,
				double maxDefocus,
				int steps);

		static gravis::d3Vector findAstigmatism(
				const aberration::EvenSolution& solution,
				const CTF& referenceCtf,
				double initialDeltaZ,
				double pixelSize,
				double initialStep);

		static std::vector<gravis::d3Vector> findMultiAstigmatism(
				const aberration::EvenSolution& solution,
				const std::vector<CTF>& referenceCtfs,
				double initialDeltaZ,
				double pixelSize,
				double lambda_reg);

		static BufferedImage<double> plotAstigmatism(
				const aberration::EvenSolution& solution,
				const CTF& referenceCtf,
				double initialDeltaZ,
				double range,
				double pixelSize,
				int size);
};

class BeerLambertFit : public Optimization
{
	public:

		BeerLambertFit(
				const std::vector<gravis::d4Matrix>& projections,
				const std::vector<double>& sum_prdObs,
				const std::vector<double>& sum_prdSqr);

			const std::vector<gravis::d4Matrix>& projections;
			const std::vector<double> &sum_prdObs, &sum_prdSqr;
			std::vector<gravis::d3Vector> view_dir;
			gravis::d3Vector tilt_p, tilt_q;

		double f(const std::vector<double>& x, void* tempStorage) const;

		double gradAndValue(const std::vector<double>& x, std::vector<double>& gradDest) const;

		double getScale(int f, const std::vector<double>& x);

};

class MultiBeerLambertFit : public FastDifferentiableOptimization
{
	public:

		MultiBeerLambertFit(
				const std::vector<std::vector<gravis::d4Matrix>>& projections,
				const std::vector<std::vector<double>>& sum_prdObs,
				const std::vector<std::vector<double>>& sum_prdSqr,
				const std::vector<double>& fractional_dose);

			const std::vector<std::vector<gravis::d4Matrix>>& projections;
			const std::vector<std::vector<double>> &sum_prdObs, &sum_prdSqr;
			const std::vector<double>& fractional_dose;
			std::vector<std::vector<gravis::d3Vector>> view_dir;
			std::vector<gravis::d3Vector> tilt_p, tilt_q;


		double gradAndValue(const std::vector<double>& x, std::vector<double>& gradDest) const;

		double getScale(int t, int f, const std::vector<double>& x);

		void report(int iteration, double cost, const std::vector<double>& x) const;

};

#endif
