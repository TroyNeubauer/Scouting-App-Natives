#include <stdio.h>
#include <libusb.h>
#include "Scouting Natives.h"

#ifdef __cplusplus
FILE iob[] = { *stdin, *stdout, *stderr };
extern "C" {
	FILE * __cdecl _iob(void) { return iob; }
}
#endif

void printdev(libusb_device *dev); //prototype of the function

int lastCount;
libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
libusb_context *ctx = NULL; //a libusb session

int count() {
	int count = libusb_get_device_list(ctx, &devs);
	libusb_free_device_list(devs, 1);
	return count;
}

/*
* Class:     com_jt_scoutserver_utils_SystemUtils
* Method:    nativeExit
* Signature: ()V
*/
;

JNIEXPORT jboolean JNICALL Java_com_jt_scoutserver_utils_SystemUtils_hasNewDevices(JNIEnv *, jclass) {
	int newCount = count();
	if (lastCount != newCount) {
		lastCount = newCount;
		return JNI_TRUE;
	}
	lastCount = newCount;
	return JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_com_jt_scoutserver_utils_SystemUtils_nativeInit(JNIEnv *, jclass) {
	if (ctx) {
		printf("[CPP] Native init already called... ctx is set. Returning!\n");
		fflush(stdout);
		return 1;
	}
	int r = libusb_init(&ctx); //initialize a library session
	if (r < 0) {
		printf("Error creating libUSB %i\n"); //there was an error
		return 1;
	}
	printf("[CPP] Initalizing the libUSB library...\n");
	fflush(stdout);
	lastCount = count();
	return 0;
}

JNIEXPORT void JNICALL Java_com_jt_scoutserver_utils_SystemUtils_nativeExit(JNIEnv * env, jclass c) {
	printf("[CPP] Closing libUSB session\n");
	fflush(stdout);
	//TODO commenting this may not be the best idea, but it avoids a nasty error
	//libusb_exit(ctx); //close the session
}

void printdev(libusb_device *dev) {
	struct libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0) {
		printf("failed to get device descriptor");
		return;
	}
	printf("Number of possible configurations: %i ", (int)desc.bNumConfigurations);
	printf("Device Class: %i ", (int)desc.bDeviceClass);
	printf("VendorID: %i ", desc.idVendor);
	printf("ProductID: %i\n", desc.idProduct);
	struct libusb_config_descriptor *config;
	if(libusb_get_config_descriptor(dev, 0, &config) == 0 && config) {
	
		printf("Interfaces: %i ||| ", (int)config->bNumInterfaces);
		const struct libusb_interface *inter;
		const struct libusb_interface_descriptor *interdesc;
		const struct libusb_endpoint_descriptor *epdesc;
		for (int i = 0; i < (int)config->bNumInterfaces; i++) {
			inter = &config->interface[i];
			printf("Number of alternate settings: %i | ", inter->num_altsetting);
			for (int j = 0; j < inter->num_altsetting; j++) {
				interdesc = &inter->altsetting[j];
				printf("Interface Number: %i | ", (int)interdesc->bInterfaceNumber);
				printf("Number of endpoints: %i | ", (int)interdesc->bNumEndpoints);
				for (int k = 0; k < (int)interdesc->bNumEndpoints; k++) {
					epdesc = &interdesc->endpoint[k];
					printf("Descriptor Type: %i | ", (int)epdesc->bDescriptorType);
					printf("EP Address: %i | ", (int)epdesc->bEndpointAddress);
				}
			}
		}
		printf("\n\n\n");
		libusb_free_config_descriptor(config);
	}
	
}