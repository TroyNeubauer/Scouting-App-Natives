#include <iostream>
#include <libusb.h>
#include <jni.h>
using namespace std;

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
	return libusb_get_device_list(ctx, &devs);
}

JNIEXPORT jboolean JNICALL Java_com_jt_scoutserver_utils_SystemUtils_hasNewDevices(JNIEnv * env, jclass c) {
	int newCount = count();
	if (lastCount != newCount) {
		lastCount = newCount;
		return JNI_TRUE;
	}
	lastCount = newCount;
	return JNI_FALSE;
}

JNIEXPORT int JNICALL Java_com_jt_scoutserver_utils_SystemUtils_nativeInit(JNIEnv * env, jclass c) {
	int r = libusb_init(&ctx); //initialize a library session
	if (r < 0) {
		cout << "Error creating libUSB " << r << endl; //there was an error
		return 1;
	}
	lastCount = count();
	return 0;
}

JNIEXPORT void JNICALL Java_com_jt_scoutserver_utils_SystemUtils_nativeExit(JNIEnv * env, jclass c) {
	libusb_free_device_list(devs, 1); //free the list, unref the devices in it
	libusb_exit(ctx); //close the session
	cout << "Closing Native libUSB session" << endl;
}

void printdev(libusb_device *dev) {
	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0) {
		cout << "failed to get device descriptor" << endl;
		return;
	}
	cout << "Number of possible configurations: " << (int)desc.bNumConfigurations << "  ";
	cout << "Device Class: " << (int)desc.bDeviceClass << "  ";
	cout << std::hex;
	cout << "VendorID: " << desc.idVendor << "  ";
	cout << "ProductID: " << desc.idProduct << endl;
	cout << std::dec;
	libusb_config_descriptor *config;
	if(libusb_get_config_descriptor(dev, 0, &config) == 0 && config) {
	
		cout << "Interfaces: " << (int)config->bNumInterfaces << " ||| ";
		const libusb_interface *inter;
		const libusb_interface_descriptor *interdesc;
		const libusb_endpoint_descriptor *epdesc;
		for (int i = 0; i < (int)config->bNumInterfaces; i++) {
			inter = &config->interface[i];
			cout << "Number of alternate settings: " << inter->num_altsetting << " | ";
			for (int j = 0; j < inter->num_altsetting; j++) {
				interdesc = &inter->altsetting[j];
				cout << "Interface Number: " << (int)interdesc->bInterfaceNumber << " | ";
				cout << "Number of endpoints: " << (int)interdesc->bNumEndpoints << " | ";
				for (int k = 0; k < (int)interdesc->bNumEndpoints; k++) {
					epdesc = &interdesc->endpoint[k];
					cout << "Descriptor Type: " << (int)epdesc->bDescriptorType << " | ";
					cout << "EP Address: " << (int)epdesc->bEndpointAddress << " | ";
				}
			}
		}
		cout << endl << endl << endl;
		libusb_free_config_descriptor(config);
	}
	
}