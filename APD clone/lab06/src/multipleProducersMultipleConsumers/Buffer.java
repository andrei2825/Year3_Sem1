//package multipleProducersMultipleConsumers;
//
//import java.util.concurrent.ArrayBlockingQueue;
//
//public class Buffer {
//	ArrayBlockingQueue<Integer> value;
//	Buffer() {
//		value = new ArrayBlockingQueue<>(100);
//	}
//	void put(int value) {
//		try{
//			this.value.put(value);
//		} catch (InterruptedException e) {
//			e.printStackTrace();
//			Thread.currentThread().interrupt();
//		}
//	}
//
//	int get() {
//		return value;
//	}
//}
