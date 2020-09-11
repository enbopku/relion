#ifndef DELETE_BLOBS_2D_PROGRAM_H
#define DELETE_BLOBS_2D_PROGRAM_H

#include <string>
#include <src/jaz/image/buffered_image.h>
#include <src/jaz/membrane/blob_fit_2d.h>

class Blob2D;
class CTF;

class DeleteBlobs2DProgram
{
	public:

		
		DeleteBlobs2DProgram(){}
		
			std::string outPath, micrographs_list_filename, micrographs_dir, blobs_dir, particles_file;
			
			bool diag, mask_other_blobs;
			
			int max_frequencies, num_threads, max_iters;
			
			double 
				prior_sigma_A,
				highpass_sigma_real_A,
				max_binning, min_binning, 
				blob_thickness,
				convergence_threshold,
				roundedness,
				smoothness,
				mask_smooth_sigma;

			std::vector<gravis::d4Vector> spheres;
			
				
		void readParameters(int argc, char *argv[]);
		void run();



	private:


		void processMicrograph(
		        int micrograph_index,
		        const std::string& micrograph_filename,
		        const std::string& blobs_filename,        
		        RawImage<float>& visualisation,
		        double visualisation_binning,
		        bool verbose,
		        CTF* ctf);

		std::vector<double> fitBlob(
		        int blob_id,
				const std::vector<double>& initial_parameters,
		        double radius_full,
				double pixel_size_full,
				double binning_factor,
				BufferedImage<float>& blob_region_full,
		        BufferedImage<float>& blob_mask_full,
				const std::string& image_name,
		        bool verbose);

		BufferedImage<float> drawFit(
				Blob2D& blob,
				const BufferedImage<float>& image,
				const BufferedImage<float>& realWeight);

		BufferedImage<float> drawTestStack(
				Blob2D& blob,
				const BufferedImage<float>& image,
				const BufferedImage<float>& realWeight);

		std::vector<double> toBin1(
				const std::vector<double>& parameters,
				double binning_factor);

		std::vector<double> fromBin1(
				const std::vector<double>& parameters,
				double binning_factor);

		BufferedImage<float> evaluateRotationalSymmetry(
				const RawImage<float>& image,
				double radius, double max_radius, double sigma);
		
		BufferedImage<int> findClosestBlob(
		        const std::vector<DelineatedBlob2D>& blobs,
		        int w, int h);
};

#endif
